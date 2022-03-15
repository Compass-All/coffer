#pragma once

#include <types.h>
#include <memory/memory.h>
#include <memory/page_table.h>

#define GET_PTE_NO_ALLOC	0
#define GET_PTE_ALLOC		1

void turn_on_mmu();
void map_page(vaddr_t vaddr, paddr_t paddr, u8 flags, u8 level);

// test
void page_table_test();