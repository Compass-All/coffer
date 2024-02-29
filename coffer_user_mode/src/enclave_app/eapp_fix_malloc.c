#include <enclave/enclave_ops.h>
#include <message/short_message.h>
#include <util/register.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE (20 * 0x200000UL)

u64 suspend()
{
	// use EXIT_ENCLAVE to exit the host daemon, but not the enclave it self
	return __ecall_ebi_suspend(EXIT_ENCLAVE);
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: %s [size]\n", argv[0]);
		exit(0);
	}

	int size = (int)strtol(argv[1], NULL, 0);

    unsigned int total_size = 0;
    unsigned long cycle = 0;

    while (1) {
        cycle = read_csr(cycle);
        total_size += size;
        printf("malloc 0x%x bytes, 0x%x in total \r\n", size, total_size);

        if (total_size > MAX_SIZE) {
            exit(0);
        }

        char *p = malloc(size);
        *p = 'a';

        if (suspend() == EXIT_ENCLAVE) {
            exit(0);
        };
    }

    return 0;
}
