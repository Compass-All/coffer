#include <host_util.h>
#include <stdio.h>

int main()
{
	u64 eid = create_enclave();
	printf("enclave created: eid = %ld\n", eid);
	fflush(stdout);
}