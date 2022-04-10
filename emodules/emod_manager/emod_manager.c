#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emodule_id.h>
#include <emodules/emodule_desc.h>
#include <message/message.h>
#include <enclave/host_ops.h>
#include <enclave/enclave_ops.h>
#include "debug/debug.h"
#include "panic/panic.h"
#include "memory/memory.h"
#include "memory/page_table.h"
#include "emod_table/emod_table.h"

#include <emodules/emod_dummy/emod_dummy.h>
#include <emodules/emod_debug/emod_debug.h>

// ---------------
// emodule manager descriptor

static emod_desc_t emod_manager_desc = {
	.emod_id = EMODULE_ID_MANAGER,
	.name = "emodule manager",
	.__signature = 0
};

static emod_manager_api_t emod_manager_api;
static emod_manager_t emod_manager;

// ---------------
// emod_manager api
static void api_test()
{
	printf("Emodule manager api testing\n");
}

// ---------------
// emod_manager init and getter

static emod_manager_t get_emodule()
{
	return emod_manager;
}

void emod_manager_init()
{
	// init emod_manager_api
	emod_manager_api.test = api_test;
	// ...
	// todo!
	show(emod_manager_api.test);

	// init emod_manager
	emod_manager.emod_manager_desc = emod_manager_desc;
	emod_manager.emod_manager_api = emod_manager_api;

	// add self to emod_table
	register_emodule(EMODULE_ID_MANAGER, (vaddr_t)get_emodule);
}

// ----- temporary implmentation -----

#define EMOD_DEBUG_LEN	0x3000
__page_aligned u8 emod_debug_buffer[EMOD_DEBUG_LEN];

static void load_emod_debug()
{
	u32 message_load_debug[2] = {
		MESSAGE_LOAD_MODULE,
		EMODULE_ID_DEBUG
	};

	u64 send_ret = __ecall_ebi_send_message(
		HOST_EID,
		(vaddr_t)&message_load_debug,
		sizeof(message_load_debug)
	);
	debug("send_ret = 0x%lx\n", send_ret);

	if (send_ret) {
		panic("send_ret error\n");
	}

	u64 listen_ret = __ecall_ebi_listen_message(
		HOST_EID,
		(vaddr_t)&emod_debug_buffer,
		sizeof(emod_debug_buffer)
	);
	debug("listen_ret = 0x%lx\n", listen_ret);

	if (listen_ret) {
		panic("listen_ret error\n");
	}

	__ecall_ebi_suspend(1UL);
	wait_until_non_zero((volatile u64 *)&emod_debug_buffer);

	show(&emod_debug_buffer);

	for (int i = 0; i < (EMOD_DEBUG_LEN >> PAGE_SHIFT); i++) {
		vaddr_t vaddr = (vaddr_t)&emod_debug_buffer + i * PAGE_SIZE;
		map_page(vaddr, vaddr - get_va_pa_offset(),
			PTE_R | PTE_W | PTE_X, SV39_LEVEL_PAGE);
	}

	vaddr_t (*debug_init)(vaddr_t) = (void *)&emod_debug_buffer;
	vaddr_t debug_module_getter_addr = debug_init((vaddr_t)get_emodule);

	register_emodule(EMODULE_ID_DEBUG, debug_module_getter_addr);
}

void emod_manager_test()
{
	load_emod_debug();

	vaddr_t debug_getter_addr = acquire_emodule(EMODULE_ID_DEBUG);
	emod_debug_t (*get_emod_debug)(void) = (void *)debug_getter_addr;
	emod_debug_t emod_debug = get_emod_debug();

	emod_debug.emod_debug_api.printd("Hello world from printd\n");
	emod_debug.emod_debug_api.printd("Int test: %d\n", 4);

	emod_debug.emod_debug_api.hexdump(
		(vaddr_t)emod_debug_buffer,
		0x20
	);

	int a = 1, b = 2;
	int c = a + b;
	int expected = 4;
	emod_debug.emod_debug_api.assert(
		(u8 *)&c,
		(u8 *)&expected,
		sizeof(expected)
	);

	return;
}