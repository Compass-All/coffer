#include <enclave/host_ops.h>
#include <stdio.h>

int main()
{
	printf("%lu\n", __ecall_get_alive_count());
}