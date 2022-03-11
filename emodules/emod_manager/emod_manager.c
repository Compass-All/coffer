#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emodule_id.h>
#include <emodules/emodule_desc.h>
#include <message/message.h>
#include <enclave/host_ops.h>
#include <enclave/enclave_ops.h>
#include "debug/debug.h"
#include "panic/panic.h"
#include "memory/memory.h"
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

	// init emod_manager
	emod_manager.emod_manager_desc = emod_manager_desc;
	emod_manager.emod_manager_api = emod_manager_api;

	// add self to emod_table
	register_emodule(EMODULE_ID_MANAGER, (vaddr_t)get_emodule);
}

// ----- temporary implmentation -----

#define EMOD_DUMMY_LEN	0x5000
#define EMOD_DEBUG_LEN	0x6000
u8 emod_dummy_buffer[EMOD_DUMMY_LEN];
u8 emod_debug_buffer[EMOD_DEBUG_LEN];

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
	debug("[load_emod_debug] send_ret = 0x%lx\n", send_ret);

	if (send_ret) {
		panic("send_ret error\n");
	}

	u64 listen_ret = __ecall_ebi_listen_message(
		HOST_EID,
		(vaddr_t)&emod_debug_buffer,
		sizeof(emod_debug_buffer)
	);
	debug("[load_emod_debug] listen_ret = 0x%lx\n", listen_ret);

	if (listen_ret) {
		panic("listen_ret error\n");
	}

	__ecall_ebi_suspend();
	wait_until_non_zero((volatile u64 *)&emod_debug_buffer);

	hexdump((vaddr_t)&emod_debug_buffer, 0x10);

	vaddr_t (*debug_init)(vaddr_t) = (void *)&emod_debug_buffer;
	vaddr_t debug_module_getter_addr = debug_init((vaddr_t)get_emodule);

	register_emodule(EMODULE_ID_DEBUG, debug_module_getter_addr);
}

static void load_emod_dummy()
{
	u32 message_load_dummy[2] = {
		MESSAGE_LOAD_MODULE,
		EMODULE_ID_DUMMY
	};

	u64 send_ret = __ecall_ebi_send_message(
		HOST_EID,
		(vaddr_t)&message_load_dummy,
		sizeof(message_load_dummy)
	);
	debug("[load_emod_dummy] send_ret = 0x%lx\n", send_ret);

	if (send_ret) {
		panic("send_ret error\n");
	}

	u64 listen_ret = __ecall_ebi_listen_message(
		HOST_EID,
		(vaddr_t)&emod_dummy_buffer,
		sizeof(emod_dummy_buffer)
	);
	debug("[load_emod_dummy] listen_ret = 0x%lx\n", listen_ret);

	if (listen_ret) {
		panic("listen_ret error\n");
	}

	__ecall_ebi_suspend();
	wait_until_non_zero((volatile u64 *)&emod_dummy_buffer);

	hexdump((vaddr_t)&emod_dummy_buffer, 0x10);

	vaddr_t (*dummy_init)(vaddr_t) = (void *)&emod_dummy_buffer;
	vaddr_t dummy_module_getter_addr = dummy_init((vaddr_t)get_emodule);

	register_emodule(EMODULE_ID_DUMMY, dummy_module_getter_addr);
}

void emod_manager_test()
{
	load_emod_debug();
	load_emod_dummy();

	vaddr_t dummy_getter_addr = acquire_emodule(EMODULE_ID_DUMMY);
	emod_dummy_t (*get_emod_dummy)(void) = (void *)dummy_getter_addr;
	emod_dummy_t emod_dummy = get_emod_dummy();
	int one_plus_two = emod_dummy.emod_dummy_api.dummy_func1(1, 2);
	int expected = 3;
	assert((void *)&one_plus_two, (void *)&expected, sizeof(int));

	debug("emod dummy id: %u\n", emod_dummy.emod_dummy_desc.emod_id);
	debug("emod name: %s\n", emod_dummy.emod_dummy_desc.name);

	vaddr_t debug_getter_addr = acquire_emodule(EMODULE_ID_DEBUG);
	emod_debug_t (*get_emod_debug)(void) = (void *)debug_getter_addr;
	emod_debug_t emod_debug = get_emod_debug();

	emod_debug.emod_debug_api.printd("Hello world from printd\n");
	emod_debug.emod_debug_api.printd("Int test: %d\n", expected);
	emod_debug.emod_debug_api.hexdump(debug_getter_addr, 0x10);

	return;
}