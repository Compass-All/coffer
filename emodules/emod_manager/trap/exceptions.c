#include "exceptions.h"
#include "syscalls.h"
#include <types.h>
#include "../panic/panic.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>
#include <util/register.h>
#include <emodules/ecall.h>
#include <enclave/enclave_ops.h>

#define SCAUSE_ECALL	0x8UL

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