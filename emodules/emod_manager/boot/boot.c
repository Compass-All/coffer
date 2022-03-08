#include <types.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/ecall.h>
#include <ebi_ecall.h>
#include <enclave/enclave_ops.h>
#include "../printf/printf.h"
#include "../debug/debug.h"
#include "../panic/panic.h"

#define TMP_STACK_SIZE	0x1000

u8 tmp_stack[TMP_STACK_SIZE];
void *const tmp_stack_top = (void *)tmp_stack + TMP_STACK_SIZE;

void emain()
{
	__ecall_ebi_suspend(); // create finished


	// enter/resume
	debug("[emain] hello world\n");

	emod_manager_init();
	emod_manager_test();

	panic("Test panic\n");
}
