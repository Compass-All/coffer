#include <host_util.h>
#include <types.h>
#include <message/message.h>
#include <emodules/emod_manager/emod_manager.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s [path_to_enclave_payload]\n", argv[0]);
		exit(0);
	}

	// create enclave
	char *payload_path = argv[1];
	
	u64 eid = create_enclave();

	// printf("enclave created: eid = %ld\n", eid);
	// fflush(stdout);

	enter_enclave(
		eid,
		argc - 1,
		argv + 1,
		payload_path
	);

	return 0;
}