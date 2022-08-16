#include "interrupts.h"
#include "../debug/debug.h"
#include "../panic/panic.h"
#include <util/register.h>
#include <util/csr.h>
#include <util/gnu_attribute.h>
#include <enclave/enclave_ops.h>
#include <message/short_message.h>

#define INTER_S_TIMER 5

#define SBI_EXT_0_1_SET_TIMER			0x0

static void timer_interrupt_handler()
{
	u64 sstatus = read_csr(sstatus);
	u64 sstatus_spie = sstatus & SSTATUS_SIE;
	clear_csr(sstatus, SSTATUS_SIE);

	u64 time = read_csr(time);
	const u64 interval = 10000UL;
	show(time);

	__ecall(
		SBI_EXT_0_1_SET_TIMER, 0UL,
		time + interval, 0UL, 0UL
	);

	clear_csr(sip, SIP_STIP);
	write_csr(sstatus, sstatus | sstatus_spie);

	static u64 count = 1;
	if (count % 100 == 0) {
		__ecall_ebi_suspend(TIMER_INTERRUPT);
	}	
	count++;
}

void interrupt_handler(
	u64 	*regs,
	u64		sepc,
	u64		scause,
	u64		stval
) {
	debug("interrupt\n");
	show(sepc);
	show(scause);
	show(stval);

	__unused u64 sip = read_csr(sip);
	show(sip);

	switch (scause)
	{
	case INTER_S_TIMER:
		debug("S TIMER Interrupt\n");
		timer_interrupt_handler();
		break;
	
	default:
		panic("Unknown interrupt\n");
		break;
	}

	debug("end of interrupt\n");
}