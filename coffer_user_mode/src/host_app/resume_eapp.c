#include <enclave/host_ops.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 4) {
        printf("Usage: %s [eid] [tid] [short message]\n",
            argv[0]);
        return 0;
    }

    int eid = atoi(argv[1]);
    int tid = atoi(argv[2]);
    int short_message = atoi(argv[3]);

    __ecall_ebi_resume((u64)eid, (u64)tid, (u64)short_message);
}