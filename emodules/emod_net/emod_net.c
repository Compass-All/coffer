#include <emodules/emod_net/emod_net.h>
#include "dependency.h"
#include "port/lwip_main.h"

// ---------------
// emodule net descriptor

static emod_desc_t emod_net_desc = {
	.emod_id = EMODULE_ID_NET,
	.name = "emodule net",
	.__signature = 0
};
static emod_net_api_t emod_net_api;
static emod_net_t emod_net;

// ---------------
// emod_net functions

static void func2(int arg1)
{
	printf("func2: %d\n", arg1);
	return;
}

// ---------------
// emod_net init and getter
static emod_net_t get_emod_net()
{
	return emod_net;
}

static void init_dependency()
{
	vaddr_t emod_debug_getter = emod_manager.emod_manager_api
		.acquire_emodule(EMODULE_ID_DEBUG);
	emod_debug_t (*get_emod_debug)(void) =
		(void *)emod_debug_getter;
	emod_debug = get_emod_debug();

	vaddr_t emod_alloc_getter = emod_manager.emod_manager_api
		.acquire_emodule(EMODULE_ID_ALLOC);
	emod_alloc_t (*get_emod_alloc)(void) =
		(void *)emod_alloc_getter;
	emod_alloc = get_emod_alloc();

	debug("Hello from init_dependency in emod_vfs\n");
}

__attribute__((section(".text.init")))
vaddr_t net_init(vaddr_t emod_manager_getter)
{
	// init api
	emod_net_api = (emod_net_api_t) {
		.test = func2
	};

	// init emodule
	emod_net = (emod_net_t) {
		.emod_net_desc = emod_net_desc,
		.emod_net_api = emod_net_api
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

	// init dependency
	init_dependency();
	// invoke lwip_main() and never returns
	lwip_main();

	return (vaddr_t)get_emod_net;
}