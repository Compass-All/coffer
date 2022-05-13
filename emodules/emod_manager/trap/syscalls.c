#include "syscalls.h"
#include <types.h>
#include "../panic/panic.h"
#include "../debug/debug.h"
#include "../memory/page_pool.h"
#include "../memory/memory.h"
#include <util/gnu_attribute.h>
#include <util/register.h>
#include <emodules/ecall.h>
#include <enclave/enclave_ops.h>
#include <emodules/emod_vfs/emod_vfs.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emodule_id.h>
#include "../memory/page_table.h"

#include <sys/stat.h>

// Syscall numbers for RISC-V
#define SYS_getcwd 			17
#define SYS_dup 			23
#define SYS_fcntl 			25
#define SYS_faccessat 		48
#define SYS_chdir 			49
#define SYS_openat 			56
#define SYS_close 			57	// 0x39
#define SYS_getdents 		61
#define SYS_lseek 			62
#define SYS_read 			63
#define SYS_write 			64	// 0x40
#define SYS_writev 			66
#define SYS_pread 			67
#define SYS_pwrite 			68
#define SYS_fstatat 		79
#define SYS_fstat 			80	// 0x50
#define SYS_exit 			93	// 0x5d
#define SYS_exit_group 		94
#define SYS_kill 			129
#define SYS_rt_sigaction 	134
#define SYS_times 			153
#define SYS_uname 			160
#define SYS_gettimeofday 	169 // 0xa9
#define SYS_getpid 			172
#define SYS_getuid 			174
#define SYS_geteuid 		175
#define SYS_getgid 			176
#define SYS_getegid 		177
#define SYS_brk 			214
#define SYS_munmap 			215
#define SYS_mremap 			216
#define SYS_mmap 			222
#define SYS_open 			1024
#define SYS_link 			1025
#define SYS_unlink 			1026
#define SYS_mkdir 			1030
#define SYS_access 			1033
#define SYS_stat 			1038
#define SYS_lstat 			1039
#define SYS_time 			1062
#define SYS_getmainvars 	2011

static emod_vfs_t emod_vfs;

__unused static void *memcpy(void *dest, const void *src, size_t count)
{
	char *temp1	  = dest;
	const char *temp2 = src;

	while (count > 0) {
		*temp1++ = *temp2++;
		count--;
	}

	return dest;
}

void load_emod_vfs()
{
	static u8 loaded = 0;
	
	if (!loaded) {
		debug("vfs not loaded\n");
		vaddr_t emod_vfs_getter = acquire_emodule(EMODULE_ID_VFS);
		emod_vfs = ((emod_vfs_t (*)(void))emod_vfs_getter)();

		loaded = 1;
	}
}

static int syscall_handler_open(const char *pathname, int flags, mode_t mode)
{
	load_emod_vfs();

	debug("pathname: %s\n", pathname);
	debug("flags = 0o%o\n", flags);
	show(mode);

	return emod_vfs.emod_vfs_api.syscall_handler_open(pathname, flags, mode);
}

static int syscall_handler_openat(
	int 		dirfd,
	const char 	*pathname,
	int 		flags,
	int 		mode)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_openat(dirfd, pathname, flags, mode);
}

static int syscall_handler_close(int fd)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_close(fd);
}

static int syscall_handler_lseek(int fd, off_t offset, int whence)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_lseek(fd, offset, whence);
}

__unused static int syscall_handler_preadv(
	int 	fd,
	const struct iovec *iov,
	int 	iovcnt,
	off_t 	offset
)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_preadv(fd, iov, iovcnt, offset);
}

static int syscall_handler_pread64(int fd, void *buf, size_t count, off_t offset)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_pread64(fd, buf, count, offset);
}

__unused static int syscall_handler_readv(int fd, const struct iovec *iov, int iovcnt)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_readv(fd, iov, iovcnt);
}

static int syscall_handler_read(int fd, void *buf, size_t count)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_read(fd, buf, count);
}

__unused static int syscall_handler_pwritev(
	int 	fd,
	const struct iovec *iov,
	int 	iovcnt,
	off_t 	offset
)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_pwritev(fd, iov, iovcnt, offset);
}

static int syscall_handler_pwrite64(
	int 	fd,
	const void *buf,
	size_t 	count,
	off_t 	offset
)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_pwrite64(fd, buf, count, offset);
}

static int syscall_handler_writev(int fd, const struct iovec *iov, int vlen)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_writev(fd, iov, vlen);
}

static int syscall_handler_write(int fd, const void *buf, size_t count)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_write(fd, buf, count);
}

// static int syscall_handler_ioctl(int fd, unsigned long int request, void *arg)
// {
// 	load_emod_vfs();

// 	return emod_vfs.emod_vfs_api
// 		.syscall_handler_ioctl(fd, request, arg);
// }

static int syscall_handler_fstat(int fd, struct stat *st)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_fstat(fd, st);
}

static int syscall_handler_fstatat(
	int			dirfd,
	const char 	*path,
	struct stat *st,
	int 		flags
)
{
	load_emod_vfs();

	return emod_vfs.emod_vfs_api
		.syscall_handler_fstatat(dirfd, path, st, flags);
}

static void *syscall_handler_mmap(
	void	*addr,
	size_t	len,
	int		prot,
	int		flags,
	int		fildes,
	off_t	off
)
{
	load_emod_vfs();
	
	return emod_vfs.emod_vfs_api
		.syscall_handler_mmap(addr, len, prot, flags, fildes, off);
}


// temporary implementation
// static int sys_fstat_handler(u64 fd, vaddr_t sstat)
// {
//     struct stat* stat = (struct stat*)sstat;
//     /* now only support stdio */
//     if (fd > 4 || fd < 0) {
//         return -1;
//     }
//     stat->st_dev = 26;
//     stat->st_ino = 6;
//     stat->st_nlink = 1;
//     stat->st_mode = S_IWUSR | S_IRUSR | S_IRGRP;
//     stat->st_uid = 1000;
//     stat->st_gid = 5;
//     stat->st_rdev = 34819;
//     stat->st_size = 0;
//     stat->st_blksize = 1024;
//     stat->st_blocks = 0;
//     return 0;
// }

void syscall_handler(
	u64 	*regs,
	u64		sepc,
	u64		scause,
	u64		stval
)
{
	u64 syscall_num = regs[CTX_INDEX_a7];
	u64 ret = 0;

	debug("handling syscall\n");
	show(syscall_num);

	switch (syscall_num)
	{
	case SYS_open:
		debug("syscall open\n");
		show(regs[CTX_INDEX_a0]);
		show(regs[CTX_INDEX_a1]);
		show(regs[CTX_INDEX_a2]);
		ret = (u64)syscall_handler_open(
			(const char *)	regs[CTX_INDEX_a0],
			(int)			regs[CTX_INDEX_a1],
			(mode_t)		regs[CTX_INDEX_a2]
		);
		debug("end of syscall open\n");
		break;

	case SYS_openat:
		debug("syscall openat\n");
		ret = (u64)syscall_handler_openat(
			(int)			regs[CTX_INDEX_a0],
			(const char *)	regs[CTX_INDEX_a1],
			(int)			regs[CTX_INDEX_a2],
			(int)			regs[CTX_INDEX_a3]
		);
		debug("end of syscall openat\n");
		break;

	case SYS_close:
		debug("syscall close\n");
		ret = (u64)syscall_handler_close(
			(int)regs[CTX_INDEX_a0]
		);
		break;

	case SYS_lseek:
		debug("syscall lseek\n");
		ret = (u64)syscall_handler_lseek(
			(int)		regs[CTX_INDEX_a0],
			(off_t)		regs[CTX_INDEX_a1],
			(int)		regs[CTX_INDEX_a2]
		);
		debug("end of syscall lseek\n");
		break;

	case SYS_pread:
		debug("syscall pread\n");
		ret = (u64)syscall_handler_pread64(
			(int)		regs[CTX_INDEX_a0],
			(void *)	regs[CTX_INDEX_a1],
			(size_t)	regs[CTX_INDEX_a2],
			(off_t)		regs[CTX_INDEX_a3]
		);
		debug("end of syscall pread\n");
		break;

	case SYS_read:
		debug("syscall read\n");
		ret = (u64)syscall_handler_read(
			(int)		regs[CTX_INDEX_a0],
			(void *)	regs[CTX_INDEX_a1],
			(size_t)	regs[CTX_INDEX_a2]
		);
		debug("end of syscall read\n");
		break;
	
	case SYS_pwrite:
		debug("syscall pwrite\n");
		ret = (u64)syscall_handler_pwrite64(
			(int)		regs[CTX_INDEX_a0],
			(void *)	regs[CTX_INDEX_a1],
			(size_t)	regs[CTX_INDEX_a2],
			(off_t)		regs[CTX_INDEX_a3]
		);
		debug("end of syscall pwrite\n");
		break;

	case SYS_writev:
		debug("syscall writev\n");
		ret = (u64)syscall_handler_writev(
			(int)					regs[CTX_INDEX_a0],
			(const struct iovec *)	regs[CTX_INDEX_a1],
			(int)					regs[CTX_INDEX_a2]
		);
		debug("end of syscall writev\n");
		break;

	case SYS_write:
		debug("syscall write\n");
		ret = (u64)syscall_handler_write(
			(int)			regs[CTX_INDEX_a0],
			(const void *)	regs[CTX_INDEX_a1],
			(size_t)		regs[CTX_INDEX_a2]
		);
		debug("end of syscall write\n");
		break;

	case SYS_fstat:
		debug("syscall fstat\n");
		ret = (u64)syscall_handler_fstat(
			(int)			regs[CTX_INDEX_a0],
			(struct stat *)	regs[CTX_INDEX_a1]
		);
		debug("end of syscall fstat\n");
		break;

	case SYS_fstatat:
		debug("syscall fstatat\n");
		ret = (u64)syscall_handler_fstatat(
			(int)			regs[CTX_INDEX_a0],
			(const char *)	regs[CTX_INDEX_a1],
			(struct stat *)	regs[CTX_INDEX_a2],
			(int)			regs[CTX_INDEX_a3]
		);
		debug("end of syscall fstatat\n");
		break;

	// case SYS_write:
		// __unused u64 fd		= regs[CTX_INDEX_a0];
	 	// char *string_ptr 	= (char *)regs[CTX_INDEX_a1];
		// usize len			= regs[CTX_INDEX_a2];

		// for (int i = 0; i < len; i++) {
		// 	_putchar(*string_ptr);
		// 	string_ptr++;
		// }
		// debug("syscall write finished\n");
		// break;

	case SYS_exit:
	case SYS_exit_group:
	 	debug("syscall exit\n");
		show(get_umode_page_pool_avail_size());
		__ecall_ebi_exit(regs[CTX_INDEX_a0]);
		break;

	case SYS_brk:
		debug("syscall brk\n");
		ret = sys_brk_handler(regs[CTX_INDEX_a0]);
		debug("end of syscall brk\n");
		break;
	
	case SYS_mmap:
		debug("syscall mmap\n");
		ret = (u64)syscall_handler_mmap(
			(void *)		regs[CTX_INDEX_a0],
			(size_t)		regs[CTX_INDEX_a1],
			(int)			regs[CTX_INDEX_a2],
			(int)			regs[CTX_INDEX_a3],
			(int)			regs[CTX_INDEX_a4],
			(off_t)			regs[CTX_INDEX_a5]
		);
		debug("end of syscall mmap\n");
		break;

	case SYS_gettimeofday:
		// todo!();
		break;

	case 29:
		break;

	case 96:
		break;
	
	case 0xDEAD:
	    paddr_t get_pa(vaddr_t va);
		volatile u8* ptr = (u8*) 0xACE00000;
		__unused paddr_t pa = get_pa((vaddr_t)ptr);
		debug("pa: %lx\n", pa);
		for (int j = 0; j < 10; j++) {
			printf("\033[37m[%s] #%d\033[0m\n", __func__, j);
	    	for (int i = 0; i < 0x200000; i++) {
				ptr[i] += i;
			}
		}
		break;
	
	default:
		panic("Unimplemented syscall\n");
		break;
	}
	show(ret);
	debug("(int)ret = %d\n", ret);
	debug("end of syscall handler\n");

	write_csr(sepc, sepc + 4);
	regs[CTX_INDEX_a0] = ret;
}