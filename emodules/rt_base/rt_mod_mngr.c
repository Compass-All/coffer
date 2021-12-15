#include "rt_mod_mngr.h"
#include <driver/driver.h>
#include "m3/page_table.h"
#include <util/memory.h>
#include "rt_console.h"

#define get_nth_mod_va(n) (ERT_EXTRA_START + n * PARTITION_SIZE)

extra_module_t extra_modules[NUM_EXTRA_MODULES];
uint32_t mod_cnt = 0;

void probe_extra_modules()
{
	int i;
	extra_module_t *emod;

	for (i = 0, emod = &extra_modules[0];
		i < NUM_EXTRA_MODULES && emod->size;
		i++, emod++) {
		
		em_debug("emod id = %u, sharable = %u\n",
			emod->id, emod->is_sharable);
		em_debug("start_addr = 0x%llx, size = 0x%lx\n",
			emod->start_addr, emod->size);
	}
}

static int setup_page_table(uintptr_t pa, uint32_t cnt, volatile extra_module_t *emod)
{
	uintptr_t va = get_nth_mod_va(cnt);
	map_page(va, pa, PAGE_UP(emod->size) >> EMEGA_PAGE_SHIFT,
		PTE_V | PTE_W | PTE_R | PTE_X, 1);

	emod->start_addr = va;

	return 0;
}

int module_init(uint32_t cnt, volatile extra_module_t *emod)
{
	setup_page_table(emod->start_addr, cnt, emod);
	
	initializer init = (initializer)emod->start_addr;
	init(emod);

	return 0;
}