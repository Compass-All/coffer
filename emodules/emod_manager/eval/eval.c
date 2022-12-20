#include "eval.h"
#include "../debug/debug.h"
#include <enclave/enclave_ops.h>

DEF_TIMER(interrupt);
DEF_TIMER(syscall);
DEF_TIMER(module);

__unused void set_s_timer()
{
	__ecall(SBI_EXT_EBI, SBI_EXT_EVAL_SET_S_TIMER,
		GET_TIMER_VALUE(interrupt),
		GET_TIMER_VALUE(syscall),
		GET_TIMER_VALUE(module)
	);
}