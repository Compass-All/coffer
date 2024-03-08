#include <enclave/host_ops.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 6) {
        printf("Invalid Input\n");
        return 0;
    }

    u64 max_pmp = (u64)atoi(argv[1]);
    u64 pmp_enable = (u64)atoi(argv[2]);
    u64 id_split = (u64)atoi(argv[3]);
    u64 tlb_cache = (u64)atoi(argv[4]);
    u64 fragmented = (u64)atoi(argv[5]);

    u64 config = pmp_enable
        | (id_split << 1UL)
        | (tlb_cache << 2UL)
        | (fragmented << 3UL);

    __ecall_eval_config(config, max_pmp);

	return 0;
}
