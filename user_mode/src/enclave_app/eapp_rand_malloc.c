#include <enclave/enclave_ops.h>
#include <util/register.h>
#include <message/short_message.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_SIZE (20 * 0x200000UL)
#define MAX_SINGLE 0x1000000

void suspend()
{
	__ecall_ebi_suspend(EXIT_ENCLAVE);
}

int main()
{
    unsigned int total_size = 0;
    unsigned long cycle = 0;

    while (1) {
        cycle = read_csr(cycle);
        unsigned int size = (unsigned int)cycle % MAX_SINGLE;
        total_size += size;
        printf("malloc 0x%x bytes, 0x%x in total \r\n", size, total_size);

        if (total_size > MAX_SIZE) {
            suspend();
        }

        char *p = malloc(size);
        *p = 'a';

        suspend();
    }

    return 0;
}
