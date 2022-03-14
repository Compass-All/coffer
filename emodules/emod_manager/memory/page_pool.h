#pragma once

#include <types.h>

#define PAGE_POOL_SMODE_OFFSET	0x0
#define PAGE_POOL_SMODE_SIZE	0x10000

#define PAGE_POOL_UMODE_OFFSET	\
	(PAGE_POOL_SMODE_OFFSET + PAGE_POOL_SMODE_SIZE)

void init_page_pool(usize offset);
paddr_t alloc_smode_page(usize number_of_pages);