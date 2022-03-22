#include <types.h>
#include "page_pool.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>
#include "memory.h"
#include "page_table.h"
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

#define U_MODE_POOL	0
#define S_MODE_POOL	1
 
// offset relative to pa_start of emod_manager
static usize page_pool_offset; // write only once
static usize page_pool_size; // write only once

static usize used_umode_page_count = 0;
static usize used_smode_page_count = 0;

const static usize PAGE_POOL_SMODE_OFFSET	= 0x0UL;
const static usize PAGE_POOL_SMODE_SIZE		= 0x100000UL;
const static usize PAGE_POOL_UMODE_OFFSET	=
	(PAGE_POOL_SMODE_OFFSET + PAGE_POOL_SMODE_SIZE);
static usize PAGE_POOL_UMODE_SIZE; // write only once

usize get_page_pool_offset()
{
	return page_pool_offset;
}

usize get_page_pool_size()
{
	return page_pool_size;
}

static usize get_used_page_count(u8 mode)
{
	if (mode == U_MODE_POOL)
		return used_umode_page_count;
	else if (mode == S_MODE_POOL)
		return used_smode_page_count;
	else
		panic("Invalid mode!\n");
}

static void set_used_page_count(usize count, u8 mode)
{
	if (mode == U_MODE_POOL)
		used_umode_page_count = count;
	else if (mode == S_MODE_POOL)
		used_smode_page_count = count;
	else
		panic("Invalid mode!\n");
}

static paddr_t get_page_pool_top(u8 mode)
{
	paddr_t pa_start;
	paddr_t page_pool_start;
	paddr_t page_pool_top;
	usize offset;
	usize used_page_count;

	offset = get_page_pool_offset();
	if (mode == U_MODE_POOL) {
		offset += PAGE_POOL_UMODE_OFFSET;
	} else if (mode == S_MODE_POOL) {
		offset += PAGE_POOL_SMODE_OFFSET;
	} else {
		panic("Invalid mode!\n");
	}

	used_page_count = get_used_page_count(mode);

	pa_start 		= get_emod_manager_pa_start();
	page_pool_start = pa_start + offset;
	page_pool_top	= page_pool_start + used_page_count * PAGE_SIZE;

	return page_pool_top;
}

void init_page_pool(usize offset, usize size)
{
	page_pool_offset 	= offset;
	page_pool_size 		= size;

	PAGE_POOL_UMODE_SIZE = size - PAGE_POOL_SMODE_SIZE;
	debug("page pool initialized\n");
	show(page_pool_offset);
	show(page_pool_size);

	__unused usize number_of_s_pages = PAGE_POOL_SMODE_SIZE / PAGE_SIZE;
	__unused usize number_of_u_pages = PAGE_POOL_UMODE_SIZE / PAGE_SIZE;
	show(number_of_s_pages);
	show(number_of_u_pages);

	paddr_t umode_page_pool_top = get_page_pool_top(U_MODE_POOL);
	paddr_t smode_page_pool_top = get_page_pool_top(S_MODE_POOL);
	show(umode_page_pool_top);
	show(smode_page_pool_top);
}

static paddr_t alloc_page(usize number_of_pages, u8 mode)
{
	paddr_t page_pool_top = get_page_pool_top(mode);
	usize	used_number_of_pages;
	usize	expected_number_of_pages;
	usize	pool_size;

	if (mode == U_MODE_POOL) {
		pool_size = PAGE_POOL_UMODE_SIZE;
	} else if (mode == S_MODE_POOL) {
		pool_size = PAGE_POOL_SMODE_SIZE;
	}

	used_number_of_pages = get_used_page_count(mode);

	expected_number_of_pages = used_number_of_pages + number_of_pages;
	if (expected_number_of_pages * PAGE_SIZE > pool_size) {
		show(number_of_pages);
		show(mode);
		panic("Pool Out Of Pages!");
	}

	set_used_page_count(expected_number_of_pages, mode);

	show(mode);
	debug("allocating %lu pages\n", number_of_pages);
	debug("%lu pages used in total, %lu pages left\n",
		expected_number_of_pages,
		(pool_size >> PAGE_SHIFT) - expected_number_of_pages);
	show(page_pool_top);

	return page_pool_top;
}

paddr_t alloc_smode_page(usize number_of_pages)
{
	return alloc_page(number_of_pages, S_MODE_POOL);
}

__unused paddr_t alloc_umode_page(usize number_of_pages)
{
	return alloc_page(number_of_pages, U_MODE_POOL);
}