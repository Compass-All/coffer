#include "exceptions.h"
#include <types.h>
#include "../panic/panic.h"
#include <util/gnu_attribute.h>

__diverge void exception_handler()
{
	panic("Trapped!");
}