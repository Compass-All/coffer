#include <emodules/emod_alloc/emod_alloc.h>
#include "dependency.h"

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
// emod_alloc functions

static int func1(int arg1, int arg2)
{
	return arg1 + arg2;
}

static void func2(int arg1)
{
	return;
}

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
	
	emod_debug.emod_debug_api.printd("Hello from init_dependency\n");
}

__attribute__((section(".text.init")))
vaddr_t alloc_init(vaddr_t emod_manager_getter)
{
	// init api
	emod_alloc_api = (emod_alloc_api_t) {
		.alloc_func1 = func1,
		.alloc_func2 = func2
	};

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