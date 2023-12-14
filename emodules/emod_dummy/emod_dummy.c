#include <emodules/emod_dummy/emod_dummy.h>
#include "tmp_printf.h"
#include "dependency.h"

// ---------------
// emodule dummy descriptor

static emod_desc_t emod_dummy_desc = {
	.emod_id = EMODULE_ID_DUMMY,
	.name = "emodule dummy",
	.__signature = 0
};
static emod_dummy_api_t emod_dummy_api;
static emod_dummy_t emod_dummy;

// ---------------
// emod_dummy functions

static int func1(int arg1, int arg2)
{
	return arg1 + arg2;
}

static void func2(int arg1)
{
	dummy_printf("func2: %d\n", arg1);
	return;
}

// ---------------
// emod_dummy init and getter
static emod_dummy_t get_emod_dummy()
{
	return emod_dummy;
}

__attribute__((section(".text.init")))
vaddr_t dummy_init(vaddr_t emod_manager_getter)
{
	// init api
	emod_dummy_api = (emod_dummy_api_t) {
		.dummy_func1 = func1,
		.dummy_func2 = func2
	};

	// init emodule
	emod_dummy = (emod_dummy_t) {
		.emod_dummy_desc = emod_dummy_desc,
		.emod_dummy_api = emod_dummy_api
	};

	// record emod_manager
	if (emod_manager_getter == (vaddr_t)0UL) {
		dummy_printf("Error: NULL emod_manager getter\n");
		return (vaddr_t)0UL;
	}

	emod_manager_t (*get_emod_manager)(void) = 
		(void *)emod_manager_getter;
	emod_manager = get_emod_manager();

	emod_manager.emod_manager_api.test();

	return (vaddr_t)get_emod_dummy;
}