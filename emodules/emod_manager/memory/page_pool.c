#include "page_pool.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>
#include "memory.h"
#include <memory/memory.h>
#include "../panic/panic.h"

/**
 * @brief a simple page pool
 * 
 * Use address relative to start pa of emod_manager only.
 * DO NOT explicitly use any virtual or physical address here
 * in order to make migration easier.
 * 
 * If any reference to va or pa is necessary, use getter
 * functions defined in "memory.h"
 * 
 * Two page pools are defined here, one for s mode and one for u mode.
 * The s mode page pool is of size 0x1_0000 (might be changed in
 * the future). All pages left are used a u mode page pool.
 */

__unused static usize used_smode_page_count = 0;
__unused static usize used_umode_page_count = 0;

static usize page_pool_offset; // write only once

static paddr_t get_smode_page_pool_top()
{
	paddr_t pa_start = get_emod_manager_pa_start();
	paddr_t smode_page_pool_start = 
		pa_start + page_pool_offset;
	paddr_t page_pool_top =
		smode_page_pool_start + used_smode_page_count * PAGE_SIZE;
	
	return page_pool_top;
}

void init_page_pool(usize offset)
{
	page_pool_offset = offset;
	debug("page pool initialized\n");
	show(page_pool_offset);
}

paddr_t alloc_smode_page(usize number_of_pages)
{
	paddr_t page_pool_top = get_smode_page_pool_top();

	usize expected_number_of_pages =
		used_smode_page_count + number_of_pages;
	
	if (expected_number_of_pages * PAGE_SIZE > PAGE_POOL_SMODE_SIZE)
		panic("S mode page pool out of page!");

	used_smode_page_count = expected_number_of_pages;

	debug("allocating %lu pages\n", number_of_pages);
	debug("%lu pages used in total, %lu pages left\n",
		expected_number_of_pages,
		(PAGE_POOL_SMODE_SIZE >> PAGE_SHIFT) - expected_number_of_pages);
	show(page_pool_top);

	return page_pool_top;
}

// void page_pool_test()
// {
// 	paddr_t page1 = alloc_smode_page(1);
// 	paddr_t page2 = alloc_smode_page(2);
// 	paddr_t page3 = alloc_smode_page(3);
// 	paddr_t page10 = alloc_smode_page(10);
// 	show(page1);
// 	show(page2);
// 	show(page3);
// 	show(page10);
// }