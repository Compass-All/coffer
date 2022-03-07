#include "tmp_printf.h"

__attribute__((section(".text.init")))
void dummy_main()
{
	printf("This is inside emod_dummy\n");

	return;
}