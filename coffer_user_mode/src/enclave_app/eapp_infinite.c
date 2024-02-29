#include <enclave_util.h>
#include <stdio.h>

int main()
{
	int count = 0;
	long a = 0;
	while (1) {
		printf("Hello, %d, a = 0x%lx\n", count++, a);
		for (int i = 0; i < 1000000000; i++) {
			a += i ^ ((i << 1) * 0x2342123UL);
		}
	}
}