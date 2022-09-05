#include "eval.h"
#include "../debug/debug.h"
#include <enclave/enclave_ops.h>

DEF_TIMER(interrupt);
DEF_TIMER(syscall);
DEF_TIMER(module);

__unused void dump_timer()
{
	// u64 eid = __ecall_ebi_get_eid();
	// printf(
	// 	"Enclave %lu: "
	// 	"interrupt: %lu,\t"
	// 	"syscall: %lu,\t"
	// 	"module: %lu\n",
	// 	eid,
	// 	GET_TIMER_VALUE(interrupt),
	// 	GET_TIMER_VALUE(syscall),
	// 	GET_TIMER_VALUE(module)
	// );
	__ecall(SBI_EXT_EBI, 0x12345UL,
		GET_TIMER_VALUE(interrupt),
		GET_TIMER_VALUE(syscall),
		GET_TIMER_VALUE(module)
	);
}