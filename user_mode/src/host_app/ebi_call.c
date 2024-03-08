#include <enclave/host_ops.h>
#include <message/short_message.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc < 2) {
        printf("Please input the ecall function id\n");
        return 0;
    }

    int fid = atoi(argv[1]);

    __ecall(SBI_EXT_EBI, (u64)fid, 0UL, 0UL, 0UL);
}