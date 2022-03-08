#include "debug.h"

void hexdump(vaddr_t addr, usize len)
{
	const u8 group_size = 4;
	u32 *ptr = (u32 *)addr;
	for (int i = 0; i < len; i += group_size * sizeof(u32),
		ptr += group_size) {

		printf("%p:\t", ptr);
		for (int j = 0; j < group_size - 1; j++)
			printf("0x%08x\t", ptr[j]);
		printf("0x%08x\n", ptr[group_size - 1]);
	}
}