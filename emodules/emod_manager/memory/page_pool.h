#pragma once

#include <types.h>
#include <util/gnu_attribute.h>

usize get_page_pool_offset();
usize get_page_pool_size();
void init_page_pool(usize offset, usize size);
paddr_t alloc_smode_page(usize number_of_pages);
__unused paddr_t alloc_umode_page(usize number_of_pages);

// test
void page_pool_test();