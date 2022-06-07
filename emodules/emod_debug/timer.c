#include "timer.h"
#include "dependency.h"
#include <util/register.h>
#include "printf.h"

#define LEN 256

static u64 cycle_record[LEN];
static u64 last_time;

static u32 count = 0;

void start_timer()
{
	count = 0;

	for (int i = 0; i < LEN; i++)
		cycle_record[i] = 0;

	last_time = read_csr(cycle);

	return;
}

void call_timer()
{
	u64 current = read_csr(cycle);
	u64 diff = current - last_time;

	cycle_record[count++] = diff;

	if (count == LEN) {
		printf_("count overflow\n");
		while (1);
	}

	last_time = current;

	return;
}

void end_timer()
{
	for (int i = 0; i < LEN; i++) {
		if (cycle_record[i])
			printf_("Check Point %d: %ld\n", i + 1, cycle_record[i]);
	}
	printf_("\n");

	return;
}