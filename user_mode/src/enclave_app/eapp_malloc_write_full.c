#include <enclave_util.h>
#include <stdio.h>
#include <enclave/enclave_ops.h>
#include <util/register.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <types.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: eapp_malloc_write [size]\n");
		exit(0);
	}

	// int size = atoi(argv[1]);
	int size = (int)strtol(argv[1], NULL, 0);
	printf("malloc 0x%x bytes\n", size);

	unsigned long cycle1 = read_csr(cycle);
	char *p = (char *)malloc(size);
	*p = 1;
	unsigned long cycle2 = read_csr(cycle);

	printf("allocating pages: cycle1 = %ld, cycle2 = %ld\n\n\n", cycle1, cycle2);
	printf("cycle diff = %ld\n\n\n", cycle2 - cycle1);

	unsigned long total = cycle2 - cycle1;

	cycle1 = read_csr(cycle);
	for (int i = 0; i < (size >> 12); i++) {
		u64 *ptr = p + (i << 12);
		
		for (int j = 0; j < 0x1000 / sizeof(u64); j++)
			ptr[j] = 1;
	}
	cycle2 = read_csr(cycle);

	printf("writing pages: cycle1 = %ld, cycle2 = %ld\n\n\n",
		cycle1, cycle2);
	printf("cycle diff = %ld\n\n\n", cycle2 - cycle1);

	total += cycle2 - cycle1;

	printf("total cycles: %ld\n\n\n", total);

	return 0;
}
