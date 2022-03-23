#include "syscalls.h"
#include <types.h>
#include "../panic/panic.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>
#include <util/register.h>
#include <emodules/ecall.h>
#include <enclave/enclave_ops.h>

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
	case 64: // sys_write
		__unused u64 fd		= regs[CTX_INDEX_a0];
	 	char *string_ptr 	= (char *)regs[CTX_INDEX_a1];
		usize len			= regs[CTX_INDEX_a2];

		for (int i = 0; i < len; i++) {
			_putchar(*string_ptr);
			string_ptr++;
		}
		debug("syscall write finished\n");
		break;

	case 93: // sys_exit
	 	debug("syscall exit\n");
		__ecall_ebi_exit(0);
		break;
	
	default:
		panic("Unimplemented syscall\n");
		break;
	}

	write_csr(sepc, sepc + 4);
	regs[CTX_INDEX_a0] = ret;
}