#include <host_util.h>
#include <types.h>
#include <message/message.h>
#include <emodules/emod_manager/emod_manager.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("Usage: %s [eid] [path_to_enclave_payload]\n", argv[0]);
		exit(0);
	}

	u64 eid = (u64)atoi(argv[1]);
	char *payload_path = argv[2];
	
	enter_enclave(
		eid,
		argc - 2,
		argv + 2,
		payload_path
	);

	return 0;
}