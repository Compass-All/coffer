/**
 * @brief EModule Manager.
 * 
 * emod_manager is aware of every emodule type.
 * However, it does not care about implementation
 * details. In other words, all emodules providing
 * the same sets of API are regarded as equivalent.
 * 
 */

#pragma once

#include <emodules/emodule_id.h>
#include <emodules/emodule_desc.h>
#include <types.h>

typedef struct {
	// test
	void (*test)(void);

	// emodule management
	vaddr_t (*acquire_emodule)(u32 emod_id);

	// memory management
	int (*memory_map)(vaddr_t vaddr, paddr_t paaddr, usize len);

	// panic
	// __diverge void panic(const char *panic_message);

	// ... to be determined
} emod_manager_api_t;

typedef struct {
	emod_desc_t emod_manager_desc;
	emod_manager_api_t emod_manager_api;
} emod_manager_t;

void emod_manager_init();
void emod_manager_test();