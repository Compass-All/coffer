#include "emod_table.h"
#include "../panic/panic.h"
#include "../debug/debug.h"
#include <types.h>

/**
 * @brief Emodule Table
 * 
 * This table contains a list of `get_emodule` function pointers.
 * The functions take no parameter and return the corresponding
 * emod_xxx_t struct.
 * 
 * A function pointer is added to the list when the corresponding
 * emodule is loaded and is never supposed to be removed from
 * the table.
 * 
 * TODO:
 * The table is currently of fixed size. It can be extended with
 * allocated memory when a memory allocation emodule is loaded.
 * 
 */

#define EMODULE_TABLE_SIZE	0x100

static vaddr_t emodule_table[EMODULE_TABLE_SIZE];

void register_emodule(u32 emod_id, vaddr_t emodule_getter_addr)
{
	if (emod_id >= EMODULE_TABLE_SIZE)
		panic("Attempt to register emodule that doesn't exist!\n");

	debug("emod %u getter at 0x%lx\n",
		emod_id, emodule_getter_addr);
	emodule_table[emod_id] = emodule_getter_addr;
}

vaddr_t acquire_emodule(u32 emod_id)
{
	if (emod_id >= EMODULE_TABLE_SIZE) {
		printf("Emodule %u does not exists\n", emod_id);
		return (vaddr_t)0UL;
	}

	vaddr_t emodule_getter_addr = emodule_table[emod_id];
	if (emodule_getter_addr == (vaddr_t)0UL)
		printf("Warning: emodule %u not loaded\n", emod_id);

	return emodule_getter_addr;
}