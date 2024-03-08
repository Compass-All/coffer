#include <enclave_util.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	char file[] = "/emodules/emod_uart.bin.signed";
	size_t filename_len = strlen(file);

	u8 content[16608];

	__ecall_5(10002, 0, (u64)&file, (u64)filename_len, (u64)&content, 16608, 0);

	for (int i = 0; i < 128; i++) {
		printf("0x%x\t", content[i]);
		if (i % 8 == 0)
			printf("\n");
	} 
}
