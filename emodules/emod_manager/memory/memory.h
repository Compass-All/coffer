#pragma once

#include <types.h>

typedef struct {
	usize	offset; // offset to pa_start of emod_manager
	usize	size;
	vaddr_t vaddr;
	u8		flags; // PTE flags
} memory_section_t;

void set_emod_manager_pa_start(paddr_t pa_start);
paddr_t get_emod_manager_pa_start();
vaddr_t get_emod_manager_va_start();
void wait_until_non_zero(volatile u64 *ptr);
void map_sections();