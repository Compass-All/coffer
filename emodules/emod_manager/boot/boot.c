#include <types.h>
#include "../printf/printf.h"
#include "../printf/debug.h"
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/ecall.h>
#include <ebi_ecall.h>
#include <enclave/enclave_ops.h>

#define TMP_STACK_SIZE	0x1000

u8 tmp_stack[TMP_STACK_SIZE];
void *const tmp_stack_top = (void *)tmp_stack + TMP_STACK_SIZE;

void emain()
{
	debug("[emain] hello world\n");

	__ecall_ebi_suspend();

	load_emodule();
}
