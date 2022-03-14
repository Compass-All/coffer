#pragma once

#include <types.h>

void set_emod_manager_pa_start(paddr_t pa_start);
paddr_t get_emod_manager_pa_start();
vaddr_t get_emod_manager_va_start();
void wait_until_non_zero(volatile u64 *ptr);