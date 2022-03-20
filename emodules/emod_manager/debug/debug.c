#include "debug.h"
#include "../panic/panic.h"

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

void assert(u8 *ptr1, u8 *ptr2, usize len)
{
	if (!ptr1 || !ptr2) {
		panic("NULL Pointer during assertion\n");
	}
	for (int i = 0; i < len; i++) {
		if (*ptr1 != *ptr2) {
			error("0x%x\t(at %p)!= 0x%x\t(at %p)",
				*ptr1, ptr1, *ptr2, ptr2);
			panic("Assert failed\n");
		}
		ptr1++;
		ptr2++;
	}
	printf("Assert passed!\n");
}