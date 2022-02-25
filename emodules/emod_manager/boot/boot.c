#include <types.h>
#include "../printf/printf.h"

#define TMP_STACK_SIZE	0x1000

u8 tmp_stack[TMP_STACK_SIZE];
void *const tmp_stack_top = (void *)tmp_stack + TMP_STACK_SIZE;

void emain()
{
	printf("Hello world\n");
}