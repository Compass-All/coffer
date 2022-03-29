#pragma once

#include <types.h>
#include <util/gnu_attribute.h>

typedef struct {
	usize	offset; // offset to pa_start of emod_manager
	usize	size;
	vaddr_t vaddr;
	u8		flags; // PTE flags
} memory_section_t;

void wait_until_non_zero(volatile u64 *ptr);

void	set_emod_manager_pa_start(paddr_t pa_start);
paddr_t	get_emod_manager_pa_start();
void	set_payload_pa_start(paddr_t pa_start);
paddr_t	get_payload_pa_start();
usize get_va_pa_offset();
void map_sections();
void init_prog_brk();
paddr_t alloc_smode_stack();
vaddr_t alloc_map_umode_stack();
u64 sys_brk_handler(vaddr_t addr);