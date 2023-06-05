#include <emodules/emod_net/emod_net.h>
#include "tmp_printf.h"
#include "dependency.h"

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

	return (vaddr_t)get_emod_net;
}