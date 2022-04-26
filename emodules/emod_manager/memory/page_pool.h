#pragma once

#include <types.h>
#include <util/gnu_attribute.h>

void init_smode_page_pool(usize offset, usize size);
void init_umode_page_pool(usize offset, usize size);
paddr_t alloc_smode_page(usize number_of_pages);
paddr_t alloc_umode_page(usize number_of_pages);
usize get_smode_page_pool_avail_size();
usize get_umode_page_pool_avail_size();

void map_smode_page_pool();

// test
void page_pool_test();