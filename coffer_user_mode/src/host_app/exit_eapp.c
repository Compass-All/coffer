#include <enclave/host_ops.h>
#include <message/short_message.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Please input the enclave id\n");
        return 0;
    }

    int id = atoi(argv[1]);

    __ecall_ebi_resume(id, 0UL, EXIT_ENCLAVE);
}