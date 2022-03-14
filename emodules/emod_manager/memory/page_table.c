#include "page_table.h"
#include "../panic/panic.h"
#include <util/gnu_attribute.h>

__unused static u8 page_table_root[PAGE_SIZE];