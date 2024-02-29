#include <enclave_util.h>
#include <stdio.h>
#include <sys/time.h>
#include <types.h>

uint64_t now_micros()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (uint64_t)(tv.tv_sec * 1000000 + tv.tv_usec);
}

u64 factorial(u64 number)
{
	if (number == 0UL || number == 1UL)
		return 1;

	u64 ret = 1;
	for (u64 i = 2UL; i <= number; i++)
		ret *= i;

	return ret;
}

int main()
{
	while (1) {
		u64 time1, time2;

		time1 = now_micros();

		u64 ans;
		for (int i = 0; i < 0x100000UL; i++)
			ans = factorial(0x30UL);
		
		time2 = now_micros();

		printf("time: %ld, ans = 0x%lx\n", time2 - time1, ans);
	}

	return 1;
}

