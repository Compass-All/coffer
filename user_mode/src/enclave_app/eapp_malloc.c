#include <enclave_util.h>
#include <stdio.h>
#include <enclave/enclave_ops.h>
#include <util/register.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <types.h>

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: eapp_malloc [size]\n");
		exit(0);
	}

	int size = atoi(argv[1]);
	printf("malloc 0x%x bytes\n", size);

	unsigned long cycle1 = read_csr(cycle);
	int *p = (int *)malloc(size);
	*p = 1;
	unsigned long cycle2 = read_csr(cycle);

	printf("cycle1 = %ld, cycle2 = %ld\n", cycle1, cycle2);
	printf("cycle diff = %ld\n", cycle2 - cycle1);

	return 0;
}
