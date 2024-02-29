#include "message/short_message.h"
#include "types.h"
#include <sbi/ebi/eval.h>
#include <sbi/sbi_console.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/sbi_system.h>
#include <enclave/eval_timer.h>

DEF_TIMER(creation); 
DEF_TIMER(execution); 
DEF_TIMER(clean_up);
DEF_TIMER(message);
DEF_TIMER(mem_alloc);
DEF_TIMER(lpmp);
DEF_TIMER(mem_migration);
DEF_TIMER(total);
DEF_S_TIMER(interrupt);
DEF_S_TIMER(syscall);
DEF_S_TIMER(emodule);
DEF_COUNTER(lpmp);
DEF_COUNTER(mem_migration);

u64 get_timer(u64 eid, u64 timer)
{
	u64 ret = 0;
	switch (timer) {
	case TIMER_CREATION:
		ret = GET_TIMER_VALUE(creation, eid);
		break;
	case TIMER_EXECUTION:
		ret = GET_TIMER_VALUE(execution, eid);
		break;
	case TIMER_CLEAN_UP:
		ret = GET_TIMER_VALUE(clean_up, eid);
		break;
	case TIMER_MESSAGE:
		ret = GET_TIMER_VALUE(message, eid);
		break;
	case TIMER_MEM_ALLOC:
		ret = GET_TIMER_VALUE(mem_alloc, eid);
		break;
	case TIMER_LPMP:
		ret = GET_TIMER_VALUE(lpmp, eid);
		break;
	case TIMER_MEM_MIGRATION:
		ret = GET_TIMER_VALUE(mem_migration, eid);
		break;
	case TIMER_TOTAL:
		ret = GET_TIMER_VALUE(total, eid);
		break;
	case TIMER_INTERRUPT:
		ret = GET_S_TIMER(interrupt, eid);
		break;
	case TIMER_SYSCALL:
		ret = GET_S_TIMER(syscall, eid);
		break;
	case TIMER_EMODULE:
		ret = GET_S_TIMER(emodule, eid);
		break;
	case COUNT_LPMP:
		ret = GET_COUNTER_VALUE(lpmp, eid);
		break;
	case COUNT_MEM_MIGRATION:
		ret = GET_COUNTER_VALUE(mem_migration, eid);
		break;
	default:
		panic("Unknown timer\n");
		break;
	}
	return ret;
}

void init_timer(u64 eid)
{
	INIT_TIMER(mem_alloc, eid);
	INIT_TIMER(lpmp, eid);
	INIT_TIMER(mem_migration, eid);
	INIT_TIMER(message, eid);
	INIT_TIMER(execution, eid);
	INIT_TIMER(creation, eid);
	INIT_TIMER(clean_up, eid);
	INIT_TIMER(total, eid);

	INIT_COUNTER(lpmp, eid);
	INIT_COUNTER(mem_migration, eid);
}

__unused void dump_timer(u64 eid)
{
	sbi_DEBUG(
		"Enclave %lu: "
		"creation: %lu, "
		"execution: %lu, "
		"clean_up: %lu, "
		"message: %lu, "
		"mem_alloc: %lu, "
		"lpmp: %lu, "
		"mem_migration: %lu, "
		"total: %lu, "
		"interrupt: %lu, "
		"syscall: %lu, "
		"emodule: %lu, "
		"lpmp count: %lu, "
		"mem_migration count: %lu\n",
		eid,
		GET_TIMER_VALUE(creation, eid),
		GET_TIMER_VALUE(execution, eid),
		GET_TIMER_VALUE(clean_up, eid),
		GET_TIMER_VALUE(message, eid),
		GET_TIMER_VALUE(mem_alloc, eid),
		GET_TIMER_VALUE(lpmp, eid),
		GET_TIMER_VALUE(mem_migration, eid),
		GET_TIMER_VALUE(total, eid),
		GET_S_TIMER(interrupt, eid),
		GET_S_TIMER(syscall, eid),
		GET_S_TIMER(emodule, eid),
		GET_COUNTER_VALUE(lpmp, eid),
		GET_COUNTER_VALUE(mem_migration, eid)
	);
}