#pragma once

#include <types.h>

#define EMODULE_MANAGER_SIZE	0x0UL
#define EMODULE_DEBUG_SIZE		0x3000
#define EMODULE_ALLOC_SIZE		0x6000
#define EMODULE_VFS_SIZE		0xD000
#define EMODULE_DUMMY_SIZE		0x5000

void 	register_emodule(u32 emod_id, vaddr_t emodule_getter_addr);
usize 	get_emodule_size(u32 emod_id);
vaddr_t get_emodule(u32 emod_id);
void	dump_emodule_table();