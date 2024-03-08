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

#include <message/short_message.h>
#include <message/message.h>
#include <emodules/emodule_id.h>
#include <util/cpu.h>

#include <types.h>

#define SLEEP_TIME 1000

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

    u32 tid = 0;
	u64 int_cnt = 0;
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
            // printf("New thread! short_message = 0x%lx\n\r", short_message);
            // printf("send_message = 0x%lx\n\r", ((u64)short_message << 32) >> 32);
            // fflush(stdout);
            if (fork() == 0) {
                int r = prctl(PR_SET_PDEATHSIG, SIGTERM);
                if (r == -1) { perror(0); exit(1); }
                if (getppid() != ppid_before_fork)
                    exit(1);
                tid = (u32)short_message;
            } else {
                send_message = ((u64)short_message << 32) >> 32; 
            }
            break;
		// case EXIT_THREAD:
		// 	printf("thread exit! short_message = 0x%lx\n\r", short_message);
        //     // printf("send_message = 0x%lx\n\r", ((u64)short_message << 32) >> 32);
		// 	break;
		default:
			break;
		}
		
		short_message = __ecall_ebi_resume(
			eid,
            (u64)tid,
			send_message
		);
	}

	printf("Enclave %lu thread %u exited, interrupt count: %lu\n", eid, tid, int_cnt);

	return eid;
}