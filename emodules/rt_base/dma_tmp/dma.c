#include "dma.h"
#include "../rt_console.h"
#include "../m3/page_table.h"


static void dma_control_page_mapping()
{
	uintptr_t addr = PDMA_BASE_ADDR;
	uintptr_t n_pages = PDMA_SIZE >> EPAGE_SHIFT;
	map_page(addr, addr, n_pages, PTE_V | PTE_W | PTE_R | PTE_D, 0);
}

static void print_dma_status()
{
	for (int i = 0; i < PDMA_NUM; i++) {
		dma_control ctrl = *(dma_control *)PDMA_CONTROL(i);
		em_debug("ID: %d\n", i);
		em_debug("claim: %d\n", ctrl.claim);
		em_debug("run: %d\n", ctrl.run);
	}
}

void dma_debug()
{
	dma_control_page_mapping();
	print_dma_status();
}