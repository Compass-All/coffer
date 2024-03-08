#include <enclave/host_ops.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    __ecall_eval_get_config();
}
