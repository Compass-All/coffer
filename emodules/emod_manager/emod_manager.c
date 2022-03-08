#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emodule_id.h>
#include <message/message.h>
#include <enclave/host_ops.h>
#include <enclave/enclave_ops.h>
#include "debug/debug.h"
#include "panic/panic.h"

#include <emodules/emod_dummy/emod_dummy.h>


#define EMOD_DUMMY_LEN	0x5000
u8 emod_dummy_buffer[EMOD_DUMMY_LEN];

void wait_for_loading_done(volatile u64 *dummy_ptr)
{
	while (*dummy_ptr == 0UL);
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
	wait_for_loading_done((volatile u64 *)&emod_dummy_buffer);

	hexdump((vaddr_t)&emod_dummy_buffer, 0x10);
}

void emod_manager_test()
{
	load_emod_dummy();

	vaddr_t (*dummy_init)(void) = (void *)&emod_dummy_buffer;
	emod_dummy_t (*get_dummy_module)(void) =
		(void *)dummy_init();
	emod_dummy_t emod_dummy = get_dummy_module();

	debug("emod dummy id: %u\n", emod_dummy.emod_dummy_desc.emod_id);
	debug("emod name: %s\n", emod_dummy.emod_dummy_desc.name);

	return;
}