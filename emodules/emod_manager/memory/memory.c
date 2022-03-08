#include "memory.h"

void wait_until_non_zero(volatile u64 *ptr)
{
	while (*ptr == 0UL);
}