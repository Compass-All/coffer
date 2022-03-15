#pragma once

#include <types.h>

#define PAGE_POOL_SMODE_OFFSET	0x0
#define PAGE_POOL_SMODE_SIZE	0x100000

#define PAGE_POOL_UMODE_OFFSET	\
	(PAGE_POOL_SMODE_OFFSET + PAGE_POOL_SMODE_SIZE)

usize get_page_pool_offset();
usize get_page_pool_size();
void init_page_pool(usize offset, usize size);
paddr_t alloc_smode_page(usize number_of_pages);

// test
void page_pool_test();