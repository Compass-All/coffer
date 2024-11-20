#include "enclave/threads.h"
#include "memory/memory.h"
#include "sys/unistd.h"
#include <stdint.h>
#include <sys/resource.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <host_util.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <malloc.h>
#include <sys/syscall.h>
#include <linux/sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#include <message/short_message.h>
#include <message/message.h>
#include <emodules/emodule_id.h>
#include <util/cpu.h>

// #include <types.h>
#include <sys/mman.h>
#include "enclave/proxy_syscalls.h"
#include "emodules/emod_net/emod_net.h"
#include <sys/socket.h>

#define SLEEP_TIME 1000

uintptr_t shared_buffer_pa = 0;
uintptr_t shared_buffer_va = 0;
size_t shared_buffer_size = 0;

usize read_file(const char *path, u8 *content[])
{
	int read_ret = 1;
	int fd;
	usize len;
	usize offset;

	if (!path) {
		printf("NULL\n");
		exit(0);
	}
	fd = open(path, O_RDONLY);
	if (fd == -1) {
		printf("File %s open failed\n", path);
		perror("Error: ");
		exit(-1);
	}

	len = lseek(fd, 0L, SEEK_END);
	lseek(fd, 0L, SEEK_SET);
	*content = (u8 *)memalign(0x1000, len);

	offset = 0;
	while (read_ret) {
		read_ret = read(fd, *content + offset, 0x400);
		if (read_ret == -1) {
			debug("File %s read failed at offset 0x%lx\n",
				path, offset);
			perror("Error: ");
			close(fd);
			free(*content);
			exit(-1);
		}
		offset += read_ret;
	}
	close(fd);

	debug("File %s len: 0x%lx\n", path, offset);

	return offset;
}

static const char emod_debug_path[] = "/emodules/emod_debug.bin.signed";
static const char emod_alloc_path[] = "/emodules/emod_alloc.bin.signed";
static const char emod_vfs_path[] 	= "/emodules/emod_vfs.bin.signed";
// static const char emod_net_path[] 	= "/emodules/emod_net.bin.signed";
static const char emod_futex_path[] 	= "/emodules/emod_futex.bin.signed";
static const char emod_dummy_path[] = "/emodules/emod_dummy.bin.signed";

static const char *emod_paths[] = {
	[EMODULE_ID_DEBUG] = emod_debug_path,
	[EMODULE_ID_ALLOC] = emod_alloc_path,
	[EMODULE_ID_VFS] 	= emod_vfs_path,
	[EMODULE_ID_FUTEX] 	= emod_futex_path,
	[EMODULE_ID_DUMMY] = emod_dummy_path,
};

static const char *get_emodule_path(u32 emodule_id)
{
	if (emodule_id >= EMODULE_ID_MAX) {
		printf("Emodule %u not found!\n", emodule_id);
		return NULL;
	}
	return emod_paths[emodule_id];
}

static int load_emodule(u32 emodule_id, u64 eid)
{
	u8		*emodule;
	usize	emodule_size;

	const char *path = get_emodule_path(emodule_id);

	emodule_size = read_file(path, &emodule);
	debug("emodule %u len = 0x%lx\n", emodule_id, emodule_size);
	debug("emodule at %p\n", emodule);

	__ecall_ebi_send_message(
		eid,
		(vaddr_t)emodule,
		emodule_size
	);

	free(emodule);

	return 0;
}

static size_t get_emodule_size(u32 emodule_id)
{
	const char *path = get_emodule_path(emodule_id);
	struct stat st;
	if (stat(path, &st)){
		printf("error getting file info: %s\n", path);
		perror("Error: ");
		exit(0);
	}
	size_t size = st.st_size;
	return size;
}

static void enclave_get_file_size(u64 short_message, u64 eid, u32 tid)
{
	u32 filename_len = (u32)short_message;
	char *filename = (char *)memalign(0x1000UL, (u64)filename_len + 1);
	filename[0] = 0;
	filename[filename_len] = 0;

	printf("filename len = %u\n", filename_len);
	fflush(stdout);

	__ecall_ebi_listen_message(eid, (vaddr_t)filename, filename_len);
	__ecall_ebi_resume(eid, (u64)tid, 0);
	printf("host get filename: %s\n", filename);
	fflush(stdout);

	struct stat st;
	size_t size;
	if (stat(filename, &st)) {
		size = -1UL;
	} else {
		size = st.st_size;
	};

	__ecall_ebi_send_message(eid, (vaddr_t)&size, sizeof(size_t));
}

static void enclave_get_file(u64 short_message, u64 eid, u32 tid)
{
	u32 filename_len = (u32)short_message;
	char *filename = (char *)memalign(0x1000UL, (u64)filename_len + 1);
	filename[0] = 0;
	filename[filename_len] = 0;

	printf("filename len = %u\n", filename_len);
	fflush(stdout);

	__ecall_ebi_listen_message(eid, (vaddr_t)filename, filename_len);
	__ecall_ebi_resume(eid, (u64)tid, 0);

	u8 *file;
	usize size;

	printf("filename: %s\n", filename);
	fflush(stdout);

	size = read_file(filename, &file);
	__ecall_ebi_send_message(eid, (vaddr_t)file, size);
}

int proxy_syscall_get_ptr_from_offset(
    proxy_data_offset offset,
    size_t data_len,
    uintptr_t* ptr
) {
  /* Validate that _shared_start+offset is sane */
  if (offset > UINTPTR_MAX - shared_buffer_va || offset >= shared_buffer_size) {
    return -1;
  }

  /* Validate that _shared_start+offset+data_len in range */
  if (data_len > UINTPTR_MAX - (shared_buffer_va + offset) ||
      data_len > shared_buffer_va - offset) {
    return -1;
  }

  /* ptr looks valid, create it */
  *ptr = shared_buffer_va + offset;

//   printf("[%s]: *ptr = 0x%lx\n", __func__, *ptr);
  return 0;
}

int proxy_syscall_args_ptr(
    struct proxy_info* proxy_info,
    uintptr_t* ptr,
    size_t* size
) {
	// printf("[%s]: proxy_info = 0x%lx\n", __func__, proxy_info);
    *size = proxy_info->call_arg_size;
	// printf("[%s]: size = %lu\n", __func__, *size);
    return proxy_syscall_get_ptr_from_offset(
        proxy_info->call_arg_offset, *size, ptr);
}
// int proxy_syscall_setup_ret();

// int proxy_syscall_check_ptr_valid(
//     uintptr_t ptr,
//     size_t data_len
// ) {
//     /* Validate start in range */
//     if (ptr > shared_buffer + shared_buffer_size || ptr < shared_buffer) {
//         return 1;
//     }

//     // Validate length
//     if (data_len > UINTPTR_MAX - ptr) {
//         return 2;
//     }

//     /* Validate end in range */
//     if (ptr + data_len > shared_buffer + shared_buffer_size) {
//         return 3;
//     }

//     return 0;
// }

// int proxy_syscall_get_offset_from_ptr(
//     uintptr_t ptr,
//     size_t data_len,
//     proxy_data_offset* offset
// ) {
//     int valid = proxy_syscall_check_ptr_valid(ptr, data_len);
    
//     if (valid != 0) return valid;
    
//     /* ptr looks valid, create it */
//     *offset = ptr - shared_buffer_va;

//     return 0;
// }

int proxy_syscall_setup_ret(
    struct proxy_info* proxy_info,
    void* ptr,
    size_t size
) {
	// printf("[%s]: ptr = 0x%lx\n", __func__, ptr);
    proxy_info->return_data.call_ret_size = size;
    // return proxy_syscall_get_offset_from_ptr(
    //     (uintptr_t)ptr, size, &proxy_info->return_data.call_ret_offset);
	proxy_info->return_data.call_ret_offset = ptr - shared_buffer_va;
	return 0;
}

static int host_handle_proxy_syscall(struct proxy_info* proxy_info)
{
	struct proxy_syscall* proxy_syscall;
	size_t args_size;
	
	// todo: solve the compilation problem.
	if (proxy_syscall_args_ptr( proxy_info,
			(uintptr_t*)&proxy_syscall, &args_size) != 0) {
	// 	// goto syscall_error;
		printf("syscall error in proxy_syscall_args_ptr\n");
	}
	
	// printf("[%s]: syscall number = %lu\n",
	// 	__func__, proxy_syscall->syscall_num);

	int64_t ret = -EPERM;
	switch (proxy_syscall->syscall_num) {
		case (SYS_prlimit64):
			sargs_SYS_prlimit64 *prlimit64_args = (sargs_SYS_prlimit64 *)proxy_syscall->data;
			struct rlimit *new_limit = prlimit64_args->new_limit_is_null? NULL: &(prlimit64_args->new_limit);
			struct rlimit *old_limit = prlimit64_args->old_limit_is_null? NULL: &(prlimit64_args->old_limit);
			ret = prlimit(prlimit64_args->pid, prlimit64_args->resource, new_limit, old_limit);
			break;
		case (SYS_socket):
			// printf("[%s]: SYS_socket\n", __func__);
			sargs_SYS_socket* socket_args = (sargs_SYS_socket *)proxy_syscall->data;
			ret = socket(socket_args->domain, socket_args->type, socket_args->protocol); 
			break;
		case (SYS_socketpair):
			printf("[%s]: skip SYS_socketpair\n", __func__);
			break;
		case (SYS_bind):
			// printf("[%s]: SYS_bind\n", __func__);
			sargs_SYS_bind *bind_args = (sargs_SYS_bind *) proxy_syscall->data; 
			ret = bind(bind_args->sockfd, (struct sockaddr *) &bind_args->addr,
						bind_args->addrlen);
			break;
		case (SYS_listen):
			// printf("[%s]: SYS_listen\n", __func__);
			sargs_SYS_listen *listen_args = (sargs_SYS_listen *) proxy_syscall->data; 
      		ret = listen(listen_args->sockfd, listen_args->backlog);
			break;
		case (SYS_accept4):
			// printf("[%s]: SYS_accept\n", __func__);
			sargs_SYS_accept4 *accept4_args = (sargs_SYS_accept4 *) proxy_syscall->data;
			struct sockaddr *accept_addr = accept4_args->addr_is_null?
									NULL: (struct sockaddr *) &accept4_args->addr;
			socklen_t *accept_addrlen = accept4_args->addrlen_is_null?
									NULL: &accept4_args->addrlen;
      		ret = accept4(accept4_args->sockfd, accept_addr,
				accept_addrlen, accept4_args->flags);
			break;
		case (SYS_connect):
			// printf("[%s]: SYS_connect\n", __func__);
			sargs_SYS_connect *connect_args = (sargs_SYS_connect *) proxy_syscall->data; 
      		ret = connect(connect_args->sockfd, (struct sockaddr *) &connect_args->addr, connect_args->addrlen);
			break;
		case (SYS_getsockname):
			// printf("[%s]: skip SYS_getsockname\n", __func__);
			sargs_SYS_getsockname *getsockname_args = (sargs_SYS_getsockname *) proxy_syscall->data;
      		ret = getsockname(getsockname_args->sockfd, (struct sockaddr *) &getsockname_args->addr, 
                	&getsockname_args->addrlen);
			break;
		case (SYS_getpeername):
			sargs_SYS_getpeername *getpeername_args = (sargs_SYS_getpeername *) proxy_syscall->data;
			ret = getpeername(getpeername_args->sockfd, (struct sockaddr *) &getpeername_args->addr, 
					&getpeername_args->addrlen);
			break;
		case (SYS_sendto):
			// printf("[%s]: SYS_sendto\n", __func__);
			sargs_SYS_sendto* sendto_args = (sargs_SYS_sendto *)proxy_syscall->data;
			struct sockaddr *dest_addr = sendto_args->dest_addr_is_null ? NULL : &sendto_args->dest_addr; 
      		socklen_t dest_addrlen = sendto_args->dest_addr_is_null ? 0 : sendto_args->addrlen; 
      		ret = sendto(sendto_args->sockfd, sendto_args->buf, sendto_args->len, sendto_args->flags, 
							dest_addr, dest_addrlen);
			break;
		case (SYS_recvfrom):
			// printf("[%s]: SYS_recvfrom\n", __func__);
			sargs_SYS_recvfrom *recvfrom_args = (sargs_SYS_recvfrom *) proxy_syscall->data; 
			struct sockaddr *src_addr = recvfrom_args->src_addr_is_null ? NULL : &recvfrom_args->src_addr; 
			socklen_t *addrlen = recvfrom_args->src_addr_is_null ? NULL : &recvfrom_args->addrlen; 
			ret = recvfrom(recvfrom_args->sockfd, recvfrom_args->buf, recvfrom_args->len, recvfrom_args->flags, 
							src_addr, addrlen);
			break;
		case (SYS_setsockopt):
			// printf("[%s]: SYS_setsockopt\n", __func__);
			sargs_SYS_setsockopt* setsockopt_args = (sargs_SYS_setsockopt *)proxy_syscall->data;
			ret = setsockopt(setsockopt_args->socket,
					setsockopt_args->level, setsockopt_args->option_name,
					&setsockopt_args->option_value, setsockopt_args->option_len); 
			break;
		case (SYS_getsockopt):
			printf("[%s]: skip SYS_getsockopt\n", __func__);
			break;
		case (SYS_shutdown):
			printf("[%s]: skip SYS_shutdown\n", __func__);
			break;
		case (SYS_sendmsg):
			printf("[%s]: skip SYS_sendmsg\n", __func__);
			break;
		case (SYS_recvmsg):
			printf("[%s]: skip SYS_recvmsg\n", __func__);
			break;
		case (SYS_readahead):
			printf("[%s]: skip SYS_readahead\n", __func__);
			break;
		case (SYS_pipe2):
			// printf("[%s]: SYS_pipe2\n", __func__);
			int *fds = (int *) proxy_syscall->data;
     		ret = pipe(fds);
			break;
		case (SYS_epoll_create1):
			// printf("[%s]: SYS_epoll_create1\n", __func__);
			sargs_SYS_epoll_create1 *epoll_args = (sargs_SYS_epoll_create1 *)proxy_syscall->data;
      		ret = epoll_create(epoll_args->size);
			break;
		case (SYS_epoll_ctl):
			// printf("[%s]: SYS_epoll_ctl\n", __func__);
			sargs_SYS_epoll_ctl *epoll_ctl_args = (sargs_SYS_epoll_ctl *) proxy_syscall->data;
     		ret = epoll_ctl(epoll_ctl_args->epfd, epoll_ctl_args->op, epoll_ctl_args->fd, (struct epoll_event * ) &epoll_ctl_args->event);
      		break;
		case (SYS_epoll_pwait):
			// printf("[%s]: SYS_epoll_epoll\n", __func__);
			sargs_SYS_epoll_pwait *epoll_pwait_args = (sargs_SYS_epoll_pwait *) proxy_syscall->data;
			ret = epoll_wait(epoll_pwait_args->epfd, &epoll_pwait_args->events, 
                epoll_pwait_args->maxevents, epoll_pwait_args->timeout);
			break;
		case (SYS_fcntl):
			sargs_SYS_fcntl *fcntl_args = (sargs_SYS_fcntl *) proxy_syscall->data;
			if (!fcntl_args->has_struct) {
				ret = fcntl(fcntl_args->fd, fcntl_args->cmd, fcntl_args->arg[0]);
			} else {
				ret = fcntl(fcntl_args->fd, fcntl_args->cmd, fcntl_args->arg);
			}
			break;
		case (SYS_read):
			sargs_SYS_read *read_args = (sargs_SYS_read *) proxy_syscall->data;
			ret = read(read_args->fd, read_args->buf, read_args->len);
			break;
		case (SYS_write):
			sargs_SYS_write *write_args = (sargs_SYS_write *) proxy_syscall->data;
			ret = write(write_args->fd, write_args->buf, write_args->len);
			break;
		case (SYS_close):
			sargs_SYS_close *close_args = (sargs_SYS_close *) proxy_syscall->data;
			ret = close(close_args->fd);
			break;
		default:
			// goto syscall_error;
			printf("Unimplement proxied syscall = %u!!\n", proxy_syscall->syscall_num);
			break;
	}
	if (ret < 0) {
		ret = -errno;  // pass -errno as the real return value to enclave
	}
	proxy_info->return_data.call_status = CALL_STATUS_OK;
	// printf("[%s]: set call status OK\n", __func__);
	
	// printf("[%s]: ret = %ld\n", __func__, ret);

	/* Setup return value */
	void* ret_data_ptr = shared_buffer_va + sizeof(struct proxy_info);
	// // no str ret
	*(int64_t *)ret_data_ptr = ret;
	if (proxy_syscall_setup_ret(proxy_info, ret_data_ptr, sizeof(int64_t)) != 0) {
	// 	// goto syscall_error;
		printf("syscall_error in setup return value\n");
	}

}

u64 create_enclave()
{
	return __ecall_ebi_create();
}

u64 enter_enclave(
	u64 		eid,
	int			argc, 
	char 		*argv[],
	const char 	*path
)
{
	u8 		*payload;
	usize 	payload_size;

	payload_size = read_file(path, &payload);
	// printf("[Enclave %lu] payload len = 0x%lx\n", eid, payload_size);
	debug("payload %s at %p\n", path, payload);

	u64 short_message = __ecall_ebi_enter(
		eid,
		(u64)argc,
		(vaddr_t)argv,
		(vaddr_t)payload,
		payload_size 
	);
	debug("short message: 0x%lx\n", short_message);

	u64 send_message = 0;
	free(payload);

    u32 tid = 1;
	u64 int_cnt = 0;
	char *stack = NULL;
	while (short_message != EXIT_ENCLAVE) {
        pid_t ppid_before_fork = getpid();
		u32 emodule_id;
        send_message = 0;
		switch (short_message & TASK_MASK)
		{
		case LOAD_MODULE:
		 	emodule_id = (u32)short_message;
			debug("Loading Emodule %u\n", emodule_id);
			load_emodule(emodule_id, eid);
			break;
		
		case GET_MODULE_SIZE:
			emodule_id = (u32)short_message;
			debug("Querying size of emodule %u\n", emodule_id);
			send_message = (u64)get_emodule_size(emodule_id);
			break;

		case GET_FILE:
			enclave_get_file(short_message, eid, tid);
			break;
		
		case GET_FILE_SIZE:
			enclave_get_file_size(short_message, eid, tid);
			break;
		
		case INTERRUPT:
			int_cnt++;
			usleep(SLEEP_TIME);
			break;

		case BLOCKED:
			usleep(SLEEP_TIME);
			break;

        case NEW_THREAD:
			char *stack = malloc(16 * 4096);
			if (stack == NULL) {
				printf("malloc failed\n");
				exit(1);  // send msg to enclave for exiting
			}
			char *stack_top = stack + 16 * 4096;
			// fork a thread sharing VM and FS
			pid_t new_tid = syscall(
				SYS_clone,
				CLONE_FILES | CLONE_FS,
				stack_top
			);
			if (new_tid <= -1) {
				perror("clone failed\n");
				free(stack);
				exit(1);  // send msg to enclave for exiting
			} else if (new_tid == 0) {
				// child thread
				// printf("child thread: new_tid = %d\n", new_tid);
				fflush(stdout);
				int r = prctl(PR_SET_PDEATHSIG, SIGTERM);
				if (r == -1) { perror(0); exit(1); }
				if (getppid() != ppid_before_fork)
					exit(1);
				tid = (u32)short_message;
				send_message = 0;   // what message to send to child thread ?
			} else {
				// parent thread
				// printf("parent thread: new_tid = %d\n", new_tid);
				fflush(stdout);
				send_message = ((u64)short_message << 32) >> 32; 
			}
            break;

			case SAVE_MSG:
				printf("[E%luT%lu]msg: %u\n", eid, tid, (u32)short_message);
				fflush(stdout);
				break;

			case PROXY_SYSCALL:
				// printf("host recv a short message : proxy syscall and short_message = 0x%lx\n", short_message);
				// fflush(stdout);
				
				// const uint64_t shared_buffer_pa = 0x280200000UL;  // PPN << 12UL
				shared_buffer_pa = ((u64)short_message & (u64)MSG_MASK) << 12;
				// printf("shared_buffer_pa = 0x%lx\n", shared_buffer_pa);

				// shared_buffer_va = -1UL;
				// static size_t shared_buffer_size = 0UL;  // unused
				// 1. if shared memory not mapped into the
				// host's Address Space, how to map it?

				// 2. another way to achieve?
				// pass the physical address and size
				// then use /dev/mem for help
				static int mem_fd;
				if (shared_buffer_va == 0UL && shared_buffer_size == 0UL) {  // first map
					// TODO: when support multi-processing, what if the child process is forked?
					mem_fd = open("/dev/mem", O_RDWR | O_SYNC);
					shared_buffer_size = 2 * PARTITION_SIZE;  // default size: 2 * 2MB
					shared_buffer_va = (uintptr_t)mmap(NULL,
						shared_buffer_size,
						PROT_READ | PROT_WRITE,
						MAP_SHARED,
						mem_fd,
						shared_buffer_pa);	// phys_addr should be page-aligned.	
					printf("shared_buffer is mapped, PA=0x%lx, VA = 0x%lx\n",\
						shared_buffer_pa, shared_buffer_va);
					fflush(stdout);
				}

				// printf("host read after mmap /dev/mem:\n");
				// for (int counter = 0; counter < 64 ; counter++) {
				// 	printf("value=0x%x\n", ((volatile char*)shared_buffer_va)[counter]);
				// 	fflush(stdout);
				// }

				struct proxy_info* proxy_info = (struct proxy_info*)shared_buffer_va;
				host_handle_proxy_syscall(proxy_info);
				break;

			default:
				break;
		}

		short_message = __ecall_ebi_resume(
			eid,
            (u64)tid,
			send_message
		);
		// printf("Enclave %lu thread %u short_message = 0x%lx\n", eid, tid, short_message);
	}

	printf("Enclave %lu thread %u exited, interrupt count: %lu\n", eid, tid, int_cnt);
	if (stack != NULL) {
		free(stack);
	}
	return eid;
}
