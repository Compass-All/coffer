#pragma once

#include <types.h>
#include <memory/memory.h>
#include <memory/page_table.h>

#define GET_PTE_NO_ALLOC	0
#define GET_PTE_ALLOC		1

// test
void page_table_test();