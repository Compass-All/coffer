#include <enclave/host_ops.h>
#include <enclave/eval_timer.h>
#include <stdio.h>
#include <stdlib.h>

const char *timer_name[TIMER_MAX] = {
	"creation",
	"execution",
	"clean_up",
	"message",
	"mem_alloc",
	"lpmp",
	"mem_migration",
	"total",
	"interrupt",
	"syscall",
	"emodule",
	"lpmp",
	"mem_migration",
};

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s [eid]\n", argv[0]);
		exit(0);
	}

	u64 eid = (u64)atoi(argv[1]);

	printf("Dump timers of enclave %lu\n", eid);
	for (u64 i = 0; i < TIMER_MAX; i++) {
		u64 timer_val = __ecall_get_timer(eid, i);
		printf("%lu: %lu\n", i, timer_val);
	}

	return 0;
}