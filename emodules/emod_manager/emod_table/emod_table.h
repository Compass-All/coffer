#pragma once

#include <types.h>

void register_emodule(u32 emod_id, vaddr_t emodule_getter_addr);
vaddr_t get_emodule(u32 emod_id);