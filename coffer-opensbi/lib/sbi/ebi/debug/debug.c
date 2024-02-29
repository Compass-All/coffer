#include "sbi/sbi_system.h"
#include <sbi/ebi/ebi_debug.h>

__unused void sbi_assert(u64 val, char *assert_msg)
{
	if (!val)
		panic(assert_msg);
}

// debug
__unused void hexdump(paddr_t addr, usize len)
{
	const u8 group_size = 4;
	u32 *ptr = (u32 *)addr;
	for (int i = 0; i < len; i += group_size * sizeof(u32),
		ptr += group_size) {

		sbi_printf("%p:\t", ptr);
		for (int j = 0; j < group_size - 1; j++)
			sbi_printf("0x%08x\t", ptr[j]);
		sbi_printf("0x%08x\n", ptr[group_size - 1]);
	}
}