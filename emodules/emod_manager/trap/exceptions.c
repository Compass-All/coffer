#include "exceptions.h"
#include "syscalls.h"
#include "interrupts.h"
#include <util/csr.h>
#include <types.h>
#include "../panic/panic.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>
#include <util/register.h>
#include <emodules/ecall.h>
#include <enclave/enclave_ops.h>
#include "../memory/page_table.h"
#include "../emod_table/emod_table.h"

#define SCAUSE_INTER	(1UL << 63)
#define SCAUSE_ECALL	0x8UL
#define dump(v) printf(#v "\t=\t0x%lx\n", (u64)(v));

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
	} else if (scause & SCAUSE_INTER) {
		interrupt_handler(regs, sepc, scause & (~SCAUSE_INTER), stval);
		return;
	}

	error("Trapped! Exception!\n");

	u64 sie = read_csr(sie);
	u64 sip = read_csr(sip);
	u64 sscratch = read_csr(sscratch);
	dump(sepc); dump(scause); dump(stval);	
	dump(sscratch); dump(sie); dump(sip);
	printf("\n");

	printf("Register Dump:\n");
	for (int i = 1; i < CTX_INDEX_MAX; i++) {
		printf("x%d:\t0x%016lx\t", i, regs[i]);

		if (i % 3 == 0)
			printf("\n");
	}
	printf("\n");

	dump_emodule_table();

	paddr_t page_fault_pa;
	paddr_t access_fault_pa;

	switch (scause)
	{
	case 0x0:
		error("Instruction address misaligned\n");
		break;
	case 0x1:
		error("Instruction access fault\n");
		break;
	case 0x2:
		error("Illegal instruction\n");
		break;
	case 0x3:
		error("Breakpoint\n");
		break;
	case 0x4:
		error("Load address misaligned\n");
		break;
	case 0x5:
		access_fault_pa = get_pa(stval);
		error("Load access fault: pa = 0x%lx\n", access_fault_pa);
		break;
	case 0x6:
		error("Store/AMO address misaligned\n");
		break;
	case 0x7:
		error("Store/AMO access fault\n");
		break;
	case 0x8:
		panic("Should never reach here\n");
		break;
	case 0x9:
		error("Environment call from S-mode\n");
		break;
	case 0xa:
		error("Reserved\n");
		break;
	case 0xb:
		error("Environment call from M-mode\n");
		break;
	case 0xc:
		error("Instruction page fault\n");
		break;
	case 0xd:
		page_fault_pa = get_pa(stval);
		LOG(page_fault_pa);
		error("Load page fault\n");
		break;
	case 0xe:
		error("Reserved for future standard use\n");
		break;
	case 0xf:
		page_fault_pa = get_pa(stval);
		LOG(page_fault_pa);
		error("Store/AMO page fault\n");
		break;
	
	default:
		break;
	}

	panic("Exception unhandled\n");
}