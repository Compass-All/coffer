#pragma once

#include <types.h>
#include <memory/memory.h>
#include <memory/page_table.h>
#include <util/gnu_attribute.h>

#define GET_PTE_NO_ALLOC	0
#define GET_PTE_ALLOC		1

extern const paddr_t linear_map_start;
extern const paddr_t linear_map_size;
extern const paddr_t linear_map_offset;

u64 init_satp();
void map_page(vaddr_t vaddr, paddr_t paddr, u8 flags, u8 level);
void setup_linear_map();

// test
paddr_t get_pa(vaddr_t va);
void page_table_test();
void test_linear_map();