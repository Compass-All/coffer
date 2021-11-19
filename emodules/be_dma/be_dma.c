#include "be_dma.h"
#include "../rt_base/rt_ecall.h"

void be_entry(uintptr_t* regs, uintptr_t scause, uintptr_t sepc,
    uintptr_t stval)
{
	ecall_putchar('h');
	ecall_putchar('\n');
	return;
}


void be_interrupt_entry(uintptr_t* regs, uintptr_t scause, uintptr_t sepc,
    uintptr_t stval)
{
	ecall_putchar('h');
	ecall_putchar('\n');
	return;
}