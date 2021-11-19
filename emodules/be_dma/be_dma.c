#include "be_dma.h"
#include "../rt_base/rt_ecall.h"

void backend_dma_driver_entry() __attribute__((section(".text.init")));
void backend_dma_driver_entry()
{
	ecall_puts("hello from backend\n");
	return;
}