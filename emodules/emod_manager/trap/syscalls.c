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
#include <time.h>

// to be implemented:
/**
 * fsync, unlink, ftruncate, fcntl, gettimeofday,
 * getpid, geteuid, getcwd, time, getdents64,
 * rt_sigaction, rt_sigprocmask, execve, uname,
 * readlink, arch_prctl, set_tid_address,
 * set_robust_list, prlimit64
 */

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
#define SYS_readv 			65
#define SYS_writev 			66
#define SYS_pread 			67
#define SYS_pwrite 			68
#define SYS_fstatat 		79
#define SYS_fstat 			80	// 0x50
#define SYS_exit 			93	// 0x5d
#define SYS_exit_group 		94
#define SYS_clock_gettime	113
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
// #define SYS_mkdir 			1030
#define SYS_mkdirat 		34	// 0x22
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

void syscall_handler_clock_gettime(__unused clockid_t clock_id, struct timespec *tp)
{
	static int init = 0;

	vaddr_t va = 0xA0000000;
	if (!init) {
		// 0x101000, 0x1000
		paddr_t pa = 0x101000;
		map_page(va, pa, PTE_R, SV39_LEVEL_PAGE);
	}

	u64 time = *(u64 *)va;

#define NSEC_PER_SEC 1000000000ULL
	tp->tv_sec	= time / NSEC_PER_SEC;
	tp->tv_nsec	= time % NSEC_PER_SEC;

	return;
}

#define DEFINE_FS_SYSCALL_HANDLER_1(type, syscall_name, type1, var1)		\
static type syscall_handler_##syscall_name(type1 var1)						\
{																			\
	load_emod_vfs();														\
	return emod_vfs.emod_vfs_api.syscall_handler_##syscall_name(var1);		\
}
#define DEFINE_FS_SYSCALL_HANDLER_2(type, syscall_name, type1, var1, type2, var2)	\
static type syscall_handler_##syscall_name(type1 var1, type2 var2)					\
{																					\
	load_emod_vfs();																\
	return emod_vfs.emod_vfs_api.syscall_handler_##syscall_name(var1, var2);		\
}
#define DEFINE_FS_SYSCALL_HANDLER_3(type, syscall_name, type1, var1, type2, var2, type3, var3)	\
static type syscall_handler_##syscall_name(type1 var1, type2 var2, type3 var3)					\
{																								\
	load_emod_vfs();																			\
	return emod_vfs.emod_vfs_api.syscall_handler_##syscall_name(var1, var2, var3);				\
}
#define DEFINE_FS_SYSCALL_HANDLER_4(type, syscall_name, type1, var1, type2, var2, type3, var3, type4, var4)	\
static type syscall_handler_##syscall_name(type1 var1, type2 var2, type3 var3, type4 var4)					\
{																											\
	load_emod_vfs();																						\
	return emod_vfs.emod_vfs_api.syscall_handler_##syscall_name(var1, var2, var3, var4);					\
}
#define DEFINE_FS_SYSCALL_HANDLER_5(type, syscall_name, type1, var1, type2, var2, type3, var3, type4, var4, type5, var5)	\
static type syscall_handler_##syscall_name(type1 var1, type2 var2, type3 var3, type4 var4, type5 var5)						\
{																															\
	load_emod_vfs();																										\
	return emod_vfs.emod_vfs_api.syscall_handler_##syscall_name(var1, var2, var3, var4, var5);								\
}
#define DEFINE_FS_SYSCALL_HANDLER_6(type, syscall_name, type1, var1, type2, var2, type3, var3, type4, var4, type5, var5, type6, var6)	\
static type syscall_handler_##syscall_name(type1 var1, type2 var2, type3 var3, type4 var4, type5 var5, type6 var6)						\
{																																		\
	load_emod_vfs();																													\
	return emod_vfs.emod_vfs_api.syscall_handler_##syscall_name(var1, var2, var3, var4, var5, var6);									\
}





DEFINE_FS_SYSCALL_HANDLER_3(int, open, const char *, pathname,
	int, flags, mode_t, mode)

DEFINE_FS_SYSCALL_HANDLER_4(int, openat, int, dirfd, const char *, pathname,
	int, flags, int, mode)

DEFINE_FS_SYSCALL_HANDLER_1(int, close, int, fd)

DEFINE_FS_SYSCALL_HANDLER_3(int, lseek, int, fd, off_t, offset, int, whence)

__unused
DEFINE_FS_SYSCALL_HANDLER_4(int, preadv, int, fd,
	const struct iovec *, iov, int, iovcnt, off_t, offset)

DEFINE_FS_SYSCALL_HANDLER_4(int, pread64, int, fd, void *, buf,
	size_t, count, off_t, offset)

__unused
DEFINE_FS_SYSCALL_HANDLER_3(int, readv, int, fd,
	const struct iovec *, iov, int, iovcnt)

DEFINE_FS_SYSCALL_HANDLER_3(int, read, int, fd, void *, buf, size_t, count)

__unused
DEFINE_FS_SYSCALL_HANDLER_4(int, pwritev, int, fd,
	const struct iovec *, iov, int, iovcnt, off_t, offset)

DEFINE_FS_SYSCALL_HANDLER_4(int, pwrite64, int, fd, const void *, buf,
	size_t, count, off_t, offset)

DEFINE_FS_SYSCALL_HANDLER_3(int, writev, int, fd, const struct iovec *, iov,
	int, vlen)

DEFINE_FS_SYSCALL_HANDLER_3(int, write, int, fd, const void *, buf, size_t, count)

DEFINE_FS_SYSCALL_HANDLER_2(int, fstat, int, fd, struct stat *, st)

DEFINE_FS_SYSCALL_HANDLER_4(int, fstatat, int, dirfd, const char *, path,
	struct stat *, st, int, flags)

DEFINE_FS_SYSCALL_HANDLER_3(int, mkdirat, int, dirfd, const char *, pathname, mode_t, mode)

DEFINE_FS_SYSCALL_HANDLER_3(int, fcntl, int, fd, unsigned int, cmd, int, arg)

DEFINE_FS_SYSCALL_HANDLER_6(void *, mmap, void *, addr, size_t, len, int, prot,
	int, flags, int, fildes, off_t, off)

DEFINE_FS_SYSCALL_HANDLER_2(int, munmap, void *, addr, size_t, len)

void syscall_handler(
	u64 	*regs,
	u64		sepc,
	u64		scause,
	u64		stval
)
{
	u64 syscall_num = regs[CTX_INDEX_a7];
	u64 ret = 0;

	debug("handling syscall %ld\n", syscall_num);
	show(syscall_num);

	switch (syscall_num)
	{
	case SYS_fcntl:
		debug("syscall open\n");
		ret = (u64)syscall_handler_fcntl(
			(int)			regs[CTX_INDEX_a0],
			(unsigned int)	regs[CTX_INDEX_a1],
			(int)			regs[CTX_INDEX_a2]
		);
		debug("end of syscall open\n");
		break;

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

	case SYS_readv:
		debug("syscall readv\n");
		ret = (u64)syscall_handler_readv(
			(int)					regs[CTX_INDEX_a0],
			(const struct iovec *)	regs[CTX_INDEX_a1],
			(int)					regs[CTX_INDEX_a2]
		);
		debug("end of syscall readv\n");
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

	case SYS_mkdirat:
		debug("syscall mkdirat\n");
		ret = (u64)syscall_handler_mkdirat(
			(int)			regs[CTX_INDEX_a0],
			(const char *)	regs[CTX_INDEX_a1],
			(mode_t)		regs[CTX_INDEX_a2]
		);
		debug("end of syscall mkdirat\n");
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
		u64 a1 = regs[CTX_INDEX_a1];
		if (a1 == 0UL) {
			debug("mmap len = 0\n");
			show(sepc);
		}
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

	case SYS_munmap:
		debug("syscall munmap\n");
		ret = (u64)syscall_handler_munmap(
			(void *)	regs[CTX_INDEX_a0],
			(size_t)	regs[CTX_INDEX_a1]
		);
		debug("end of syscall munmap\n");
		break;

	case SYS_clock_gettime:
		debug("syscall clock_gettime\n");
		syscall_handler_clock_gettime(
			(clockid_t)			regs[CTX_INDEX_a0],
			(struct timespec *)	regs[CTX_INDEX_a1]
		);
		debug("end of syscall clock_gettime\n");
		break;

	// case SYS_gettimeofday:
	// 	// todo!();
	// 	break;

	case 29:
	case 96:
		break;
	
	default:
		error("syscall %d\n", syscall_num);
		show(regs[CTX_INDEX_a0]);
		show(regs[CTX_INDEX_a1]);
		show(regs[CTX_INDEX_a2]);
		show(regs[CTX_INDEX_a3]);
		show(regs[CTX_INDEX_a4]);
		show(regs[CTX_INDEX_a5]);

		panic("Unimplemented syscall\n");
		break;
	}
	show(ret);
	debug("(int)ret = %d\n", ret);
	debug("end of %ld syscall handler\n", syscall_num);

	write_csr(sepc, sepc + 4);
	regs[CTX_INDEX_a0] = ret;
}