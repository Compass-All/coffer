#pragma once

#include <types.h>
#include <util/gnu_attribute.h>

usize get_page_pool_offset();
usize get_page_pool_size();
void init_page_pool(usize offset, usize size);
paddr_t alloc_smode_page(usize number_of_pages);
paddr_t alloc_umode_page(usize number_of_pages);
usize get_umode_page_pool_avail_size();
usize get_smode_page_pool_avail_size();

// test
void page_pool_test();