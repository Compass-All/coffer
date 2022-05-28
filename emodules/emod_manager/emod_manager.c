#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emodule_id.h>
#include <emodules/emodule_desc.h>
#include <message/message.h>
#include <message/short_message.h>
#include <enclave/host_ops.h>
#include <enclave/enclave_ops.h>
#include "debug/debug.h"
#include "panic/panic.h"
#include "memory/memory.h"
#include "memory/page_table.h"
#include "memory/page_pool.h"
#include "emod_table/emod_table.h"

#include <emodules/emod_dummy/emod_dummy.h>
#include <emodules/emod_debug/emod_debug.h>
#include <emodules/emod_alloc/emod_alloc.h>

// ---------------
// emodule manager descriptor

static emod_desc_t emod_manager_desc = {
	.emod_id = EMODULE_ID_MANAGER,
	.name = "emodule manager",
	.__signature = 0
};

static emod_manager_api_t 	emod_manager_api;
static emod_manager_t 		emod_manager;

// ---------------
// emod_manager api

static emod_manager_t get_emod_manager();

static void api_test()
{
	debug("Emodule manager api testing\n");
	return;
}

static void load_emodule(u32 emodule_id)
{
	show(emodule_id);

	usize emodule_size = get_emodule_size(emodule_id);	
	show(emodule_size);

	vaddr_t vaddr = alloc_map_emodule(emodule_size);
	show(vaddr);

	__ecall_ebi_listen_message(
		0UL,
		vaddr,
		emodule_size	
	);

	__ecall_ebi_suspend(LOAD_MODULE | emodule_id);
	wait_until_non_zero((volatile u64 *)vaddr);

	vaddr_t (*init)(vaddr_t) = (void *)vaddr;
	vaddr_t getter_addr = init((vaddr_t)get_emod_manager);

	show(getter_addr);

	register_emodule(emodule_id, getter_addr);
}

vaddr_t acquire_emodule(u32 emodule_id)
{
	vaddr_t emodule_getter_addr = get_emodule(emodule_id);

	if (!emodule_getter_addr) {
		debug("loading emodule\n");
		show(emodule_id);

		load_emodule(emodule_id);

		emodule_getter_addr = get_emodule(emodule_id);
	}

	show(emodule_getter_addr);

	return emodule_getter_addr;	
}

// ---------------
// emod_manager init and getter

static emod_manager_t get_emod_manager()
{
	return emod_manager;
}

void emod_manager_init()
{
	// init emod_manager_api
	emod_manager_api.test 				= api_test;
	emod_manager_api.acquire_emodule 	= acquire_emodule;
	emod_manager_api.map_page			= map_page;
	emod_manager_api.panic				= panic;
	emod_manager_api.get_pa				= get_pa;
	// ...
	// todo!
	show(emod_manager_api.test);
	show(emod_manager_api.acquire_emodule);

	// init emod_manager
	emod_manager.emod_manager_desc = emod_manager_desc;
	emod_manager.emod_manager_api = emod_manager_api;

	// add self to emod_table
	register_emodule(EMODULE_ID_MANAGER, (vaddr_t)get_emod_manager);
}

// ----- temporary implmentation -----
__unused void emod_manager_test()
{
	load_emodule(EMODULE_ID_ALLOC);

	vaddr_t emod_alloc_getter = acquire_emodule(EMODULE_ID_ALLOC);
	emod_alloc_t emod_alloc = ((emod_alloc_t (*)(void))emod_alloc_getter)();
	char *allocated_addr = emod_alloc.emod_alloc_api.malloc(0x1000);

	show(allocated_addr);

	u64 *ptr = (u64 *)allocated_addr;
	*ptr = 0xdeadcafe;
	show(*ptr);

	return;
}