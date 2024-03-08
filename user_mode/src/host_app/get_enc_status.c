#include <enclave/host_ops.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Please input the enclave id\n");
        return 0;
    }

    u64 eid = (u64)atoi(argv[1]);

	printf("%lu\n", __ecall_get_enclave_status(eid));

	return 0;
}