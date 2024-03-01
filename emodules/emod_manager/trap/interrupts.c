#include "interrupts.h"
#include "../debug/debug.h"
#include "../panic/panic.h"
#include "../eval/eval.h"
#include <util/register.h>
#include <util/csr.h>
#include <util/gnu_attribute.h>
#include <util/console.h>
#include <enclave/enclave_ops.h>
#include <message/short_message.h>
#include "emodules/grand_lock.h"

#define INTER_S_TIMER 5

#define SBI_EXT_0_1_SET_TIMER			0x0

static void routine(u64 *regs, u64 sepc)
{
	// printf(KMAG "[interrupt rountine] spec: 0x%lx\n" RESET, sepc);
	return;
}

static void timer_interrupt_handler()
{
	START_TIMER(interrupt);
	
	u64 sstatus = read_csr(sstatus);
	u64 sstatus_spie = sstatus & SSTATUS_SIE;
	clear_csr(sstatus, SSTATUS_SIE);

	u64 time = read_csr(time);
	const u64 interval = 10000UL;

	__ecall(
		SBI_EXT_0_1_SET_TIMER, 0UL,
		time + interval, 0UL, 0UL
	);

	clear_csr(sip, SIP_STIP);
	write_csr(sstatus, sstatus | sstatus_spie);

	static u64 count = 1;
	if (count % 10 == 0) {
		STOP_TIMER(interrupt);
        spin_unlock_grand();
		// __ecall_ebi_suspend(INTERRUPT);
        spin_lock_grand_suspend();
		START_TIMER(interrupt);
	}	
	count++;

	STOP_TIMER(interrupt);
}

void interrupt_handler(
	u64 	*regs,
	u64		sepc,
	u64		scause,
	u64		stval
) {
	switch (scause)
	{
	case INTER_S_TIMER:
		timer_interrupt_handler();
		routine(regs, sepc);
		break;
	
	default:
		panic("Unknown interrupt\n");
		break;
	}
}