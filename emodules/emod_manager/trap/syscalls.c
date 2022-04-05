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

// temporary implementation
static int sys_fstat_handler(u64 fd, vaddr_t sstat)
{
    struct stat* stat = (struct stat*)sstat;
    /* now only support stdio */
    if (fd > 4 || fd < 0) {
        return -1;
    }
    stat->st_dev = 26;
    stat->st_ino = 6;
    stat->st_nlink = 1;
    stat->st_mode = S_IWUSR | S_IRUSR | S_IRGRP;
    stat->st_uid = 1000;
    stat->st_gid = 5;
    stat->st_rdev = 34819;
    stat->st_size = 0;
    stat->st_blksize = 1024;
    stat->st_blocks = 0;
    return 0;
}

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
	case SYS_close:
		// todo!();
		break;

	case SYS_write:
		__unused u64 fd		= regs[CTX_INDEX_a0];
	 	char *string_ptr 	= (char *)regs[CTX_INDEX_a1];
		usize len			= regs[CTX_INDEX_a2];

		for (int i = 0; i < len; i++) {
			_putchar(*string_ptr);
			string_ptr++;
		}
		debug("syscall write finished\n");
		break;

	case SYS_fstat:
		ret = sys_fstat_handler(
			regs[CTX_INDEX_a0],
			regs[CTX_INDEX_a1]
		);
		break;

	case SYS_exit:
	 	debug("syscall exit\n");
		show(get_umode_page_pool_avail_size());
		__ecall_ebi_exit(regs[CTX_INDEX_a0]);
		break;

	case SYS_brk:
		debug("syscall brk\n");
		ret = sys_brk_handler(regs[CTX_INDEX_a0]);
		break;

	case SYS_gettimeofday:
		// todo!();
		break;
	
	default:
		panic("Unimplemented syscall\n");
		break;
	}

	write_csr(sepc, sepc + 4);
	regs[CTX_INDEX_a0] = ret;
}