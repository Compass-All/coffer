#include "rt_mod_mngr.h"
#include <driver/driver.h>
#include "m3/page_table.h"
#include <util/memory.h>
#include "rt_console.h"

#define get_nth_mod_va(n) (ERT_EXTRA_START + n * PARTITION_SIZE)
static uintptr_t reg_va_top = ERT_PERI_REG_START;

extra_module_t extra_modules[NUM_EXTRA_MODULES];
uint32_t mod_cnt = 0;

// emod->start_addr and emod->peripheral changed
// from pa to va after invoking this function
static int setup_page_table(uintptr_t pa, uint32_t cnt,
	volatile extra_module_t *emod)
{
	// setup the page mapping of the modules
	uintptr_t module_va = get_nth_mod_va(cnt);
	map_page(module_va, pa, PAGE_UP(emod->size) >> EPAGE_SHIFT,
		PTE_V | PTE_W | PTE_R | PTE_X, 1);

	emod->start_addr = module_va;

	// setup the page mapping of the peripheral registers
	for (int i = 0; i < PERI_REGION_MAX; i++) {
		if (emod->peripheral.peri_region[i].size) {
			uintptr_t reg_pa = emod->peripheral.peri_region[i].start;
			uintptr_t reg_size = emod->peripheral.peri_region[i].size;
			map_page(reg_va_top, reg_pa, PAGE_UP(reg_size) >> EPAGE_SHIFT,
				PTE_V | PTE_W | PTE_R, 1);

			// update to va
			emod->peripheral.peri_region[i].start = reg_va_top;

			reg_va_top += PAGE_UP(reg_size) + EPAGE_SIZE;
		}
	}

	return 0;
}

int module_init(uint32_t cnt, volatile extra_module_t *emod)
{
	setup_page_table(emod->start_addr, cnt, emod);

	initializer init = (initializer)emod->start_addr;
	init(emod);

	em_debug("emod id = %u, is_sharable: %d\n",
		emod->id, emod->is_sharable);
	em_debug("handler at 0x%p\n", emod->handler);
	em_debug("interrupt_handler at 0x%p\n", emod->interrupt_handler);

	peripheral_t p = emod->peripheral;
	for (int i = 0; i < PERI_REGION_MAX; i++) {
		if (p.peri_region[i].size) {
			em_debug("region %d: start = 0x%lx, size = 0x%lx\n",
				i, p.peri_region[i].size, p.peri_region[i].start);
		}
	}

	// module_arg_t arg; arg.cmd = 0;
	// emod->handler(arg);
	// emod->interrupt_handler(arg);

	return 0;
}

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

		module_init(mod_cnt++, emod);
	}
}

// to be optimized: use a table to avoid searching every time
extra_module_t *get_emod_by_id(uint32_t mod_id)
{
	int i;
	extra_module_t *emod;

	for (i = 0, emod = &extra_modules[0];
		i < NUM_EXTRA_MODULES && emod->size;
		i++, emod++) {
		
		if (emod->id == mod_id)
			return emod;
	}

	return NULL;
}