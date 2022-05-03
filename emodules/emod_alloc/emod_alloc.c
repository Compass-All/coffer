#include <emodules/emod_alloc/emod_alloc.h>
#include "dependency.h"
#include "alloc.h"

// ---------------
// emodule alloc descriptor

static emod_desc_t emod_alloc_desc = {
	.emod_id = EMODULE_ID_ALLOC,
	.name = "emodule alloc",
	.__signature = 0
};
static emod_alloc_api_t emod_alloc_api;
static emod_alloc_t 	emod_alloc;

// ---------------
// emod_alloc init and getter
static emod_alloc_t get_emod_alloc()
{
	return emod_alloc;
}

static void init_dependency()
{
	vaddr_t emod_debug_getter = emod_manager.emod_manager_api
		.acquire_emodule(EMODULE_ID_DEBUG);
	emod_debug_t (*get_emod_debug)(void) =
		(void *)emod_debug_getter;
	emod_debug = get_emod_debug();
	
	debug("Hello from init_dependency in emod_alloc\n");
}

__attribute__((section(".text.init")))
vaddr_t alloc_init(vaddr_t emod_manager_getter)
{
	// init api
	emod_alloc_api.kmalloc 	= kmalloc;
	emod_alloc_api.malloc 	= malloc;
	emod_alloc_api.calloc 	= calloc;
	emod_alloc_api.free		= free;

	// init emodule
	emod_alloc = (emod_alloc_t) {
		.emod_alloc_desc = emod_alloc_desc,
		.emod_alloc_api = emod_alloc_api
	};

	// record emod_manager
	if (emod_manager_getter == (vaddr_t)0UL) {
		return (vaddr_t)0UL;
	}

	emod_manager_t (*get_emod_manager)(void) = 
		(void *)emod_manager_getter;
	emod_manager = get_emod_manager();

	emod_manager.emod_manager_api.test();

	init_dependency();

	return (vaddr_t)get_emod_alloc;
}