#include <enclave_util.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("argc at %p\n", &argc);
	printf("argc = %d\n", argc);
	printf("argv points to %p\n", argv);

	for (int i = 0; i < argc; i++) {
		printf("argv[%d] = %p\n", i, argv[i]);
		printf("argv[%d]: %s\n", i, argv[i]);
	}
}
