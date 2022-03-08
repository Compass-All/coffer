#include <emodules/emod_dummy/emod_dummy.h>
#include "tmp_printf.h"

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
	printf("func2: %d\n", arg1);
	return;
}

// ---------------
// emod_dummy init and query
static emod_dummy_t get_emodule()
{
	return emod_dummy;
}

__attribute__((section(".text.init")))
vaddr_t dummy_init()
{
	emod_dummy_api.dummy_func1 = func1;
	emod_dummy_api.dummy_func2 = func2;

	emod_dummy.emod_dummy_desc = emod_dummy_desc;
	emod_dummy.emod_dummy_api = emod_dummy_api;
	
	return (vaddr_t)get_emodule;
}