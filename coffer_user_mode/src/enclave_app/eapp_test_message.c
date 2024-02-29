#include <enclave_util.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	int i = 1;
	while (i < 3) {
		printf("test message: %d\n", i++);
		suspend_enclave_with_message(i);
	}
}
