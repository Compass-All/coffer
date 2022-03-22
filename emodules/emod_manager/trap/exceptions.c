#include "exceptions.h"
#include <types.h>
#include "../panic/panic.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>
#include <util/register.h>
#include <emodules/ecall.h>
#include <enclave/enclave_ops.h>

// temporary implementation of syscall handler
#define SCAUSE_ECALL	0x8UL
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

void exception_handler(
	u64* 	regs,
	u64		sepc,
	u64		scause,
	u64		stval
)
{
	if (scause == SCAUSE_ECALL) {
		syscall_handler(regs, sepc, scause, stval);
		return;
	}

	error("Trapped! Exception!\n");

	show(sepc); show(scause); show(stval);	
	printf("\n");

	printf("Register Dump:\n");
	for (int i = 1; i < CTX_INDEX_MAX; i++) {
		printf("x%d:\t0x%016lx\t", i, regs[i]);

		if (i % 3 == 0)
			printf("\n");
	}
	printf("\n");

	panic("Exception unhandled\n");
}