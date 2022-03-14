#include "memory.h"

// initialized during boot
volatile 	static paddr_t emod_manager_pa_start;
// defined in config.mk
const 		static vaddr_t emod_manager_va_start = EMOD_MANAGER_SVA_START;

void set_emod_manager_pa_start(paddr_t pa_start)
{
	emod_manager_pa_start = pa_start;
}

paddr_t get_emod_manager_pa_start()
{
	return emod_manager_pa_start;
}

vaddr_t get_emod_manager_va_start()
{
	return emod_manager_va_start;
}

void wait_until_non_zero(volatile u64 *ptr)
{
	while (*ptr == 0UL);
}