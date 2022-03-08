#include "panic.h"
#include "../debug/debug.h"

__attribute__((noreturn)) void panic(const char *panic_message)
{
	error("%s", panic_message);

	while (1);
}