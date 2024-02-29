#include <host_util.h>
#include <types.h>
#include <message/message.h>
#include <emodules/emod_manager/emod_manager.h>
#include <util/register.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s [path_to_enclave_payload]\n", argv[0]);
		exit(0);
	}

	u64 time1, time2;

	time1 = read_csr(time);

	system(argv[1]);

	time2 = read_csr(time);

	printf("Execution time: %lu\n", time2 - time1);

	return 0;
}