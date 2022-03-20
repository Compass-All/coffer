#include "panic.h"
#include "../debug/debug.h"

__diverge __unused void panic(const char *panic_message)
{
	error("%s", panic_message);

	while (1);
}

__diverge __unused void todo()
{
	panic("Unimplemented!\n");
}