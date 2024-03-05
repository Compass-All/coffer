#include <emodules/emod_futex/emod_futex.h>
#include "futex.h"
#include "emodules/emodule_id.h"
#include "dependency.h"

// ---------------
// emodule futex descriptor

static emod_desc_t emod_futex_desc = {
	.emod_id = EMODULE_ID_FUTEX,
	.name = "emodule futex",
};
static emod_futex_api_t emod_futex_api;
static emod_futex_t emod_futex;

// ---------------
// emod_futex init and getter
static emod_futex_t get_emod_futex()
{
	return emod_futex;
}

static void init_dependency()
{
	vaddr_t emod_debug_getter = emod_manager.emod_manager_api
		.acquire_emodule(EMODULE_ID_DEBUG);
	emod_debug_t (*get_emod_debug)(void) =
		(void *)emod_debug_getter;
	emod_debug = get_emod_debug();

	debug("Hello from init_dependency in emod_futex\n");
}

__attribute__((section(".text.init")))
vaddr_t emod_futex_init(vaddr_t emod_manager_getter)
{
	// init api
	emod_futex_api = (emod_futex_api_t) {
		.sys_futex_handler = sys_futex_handler
	};

	// init emodule
	emod_futex = (emod_futex_t) {
		.emod_futex_desc = emod_futex_desc,
		.emod_futex_api = emod_futex_api
	};

	// record emod_manager
	if (emod_manager_getter == (vaddr_t)0UL) {
		printf("Error: NULL emod_manager getter\n");
		return (vaddr_t)0UL;
	}

	emod_manager_t (*get_emod_manager)(void) = 
		(void *)emod_manager_getter;
	emod_manager = get_emod_manager();

	emod_manager.emod_manager_api.test();

	init_dependency();
    futex_init();

	return (vaddr_t)get_emod_futex;
}
