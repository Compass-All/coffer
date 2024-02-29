#include <enclave_util.h>
#include <types.h>

#include <stdio.h>
#include <sys/time.h>


u64 now_micros() {
	struct timeval tv;
	gettimeofday(&tv, NULL);

	return (u64)(tv.tv_sec * 1000000 + tv.tv_usec);
}

int main()
{
	u64 start = now_micros();
	while (1) {
		u64 now = now_micros() - start;

		printf("%ld\n", now);

		if (now > 100000000000)
			break;
	}
}