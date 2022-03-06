#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emodule_info.h>
#include <message/message.h>
#include <enclave/host_ops.h>
#include <enclave/enclave_ops.h>
#include "printf/debug.h"

const u32 emod_id = 1;
#define len 0x2c00UL
volatile u32 emod[len];

void load_emodule()
{
	u32 message[2] = { MESSAGE_LOAD_MODULE, emod_id };

	// setup rx channel
	int listen_ret = (int)__ecall_ebi_listen_message(
		HOST_EID,
		(vaddr_t)&emod,
		len
	);
	debug("listen message ret = %d\n", listen_ret);

	// send message via tx channel
	int send_ret = (int)__ecall_ebi_send_message(
		HOST_EID,
		(vaddr_t)&message,
		2 * sizeof(u32)
	);
	debug("send message ret = %d\n", send_ret);

	__ecall_ebi_suspend();
	while (!emod[0]);

	debug("emodule received! emod_id = %d\n", emod_id);

	printf("emod dump:\n");

	// TODO: implement a hexdump function or macro
	// #define hexdump(print_func, addr, len)
	for (int i = 0; i < len / sizeof(u32); i += 4) {
		printf(
			"0x%x:\t"
			"0x%08x\t""0x%08x\t""0x%08x\t""0x%08x\n",
			i * sizeof(u32),
			emod[i], emod[i+1], emod[i+2], emod[i+3]
		);
	}
}