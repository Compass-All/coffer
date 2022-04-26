#include "page_pool.h"
#include "memory.h"
#include "page_table.h"
#include "../panic/panic.h"
#include "../debug/debug.h"

/**
 * @brief a simple page pool
 * 
 * Use address relative to start pa of emod_manager/payload only.
 * DO NOT explicitly use any virtual or physical address here
 * in order to make migration easier.
 * 
 * If any reference to va or pa is necessary, use getter
 * functions defined in "memory.h"
 * 
 * Two page pools are defined here, one for s mode and one for u mode.
 * The s mode pool utilizes the remained memory in the partition of
 * the emod_manager. The u mode pool uses the remained memory in the
 * partition of the ELF payload.
 */

#define UMODE_POOL	0
#define SMODE_POOL	1

typedef struct {
	usize offset;	// offset relative to start of the partition
	usize size;		// should be page aligned
	usize used_page_count;
} pool_desc_t;

static pool_desc_t umode_pool_desc;
static pool_desc_t smode_pool_desc;

void init_smode_page_pool(usize offset, usize size)
{
	smode_pool_desc = (pool_desc_t) {
		.offset = offset,
		.size	= size,
		.used_page_count = 0
	};

	show(offset);
	show(size);
}

void init_umode_page_pool(usize offset, usize size)
{
	umode_pool_desc = (pool_desc_t) {
		.offset = offset,
		.size	= size,
		.used_page_count = 0
	};

	show(offset);
	show(size);
}

static usize get_used_page_count(u8 mode)
{
	if (mode == SMODE_POOL)
		return smode_pool_desc.used_page_count;
	else if (mode == UMODE_POOL)
		return umode_pool_desc.used_page_count;
	else
		panic("Invalid mode!\n");
}

static void set_used_page_count(usize count, u8 mode)
{
	if (mode == SMODE_POOL)
		smode_pool_desc.used_page_count = count;
	else if (mode == UMODE_POOL)
		umode_pool_desc.used_page_count = count;
	else
		panic("Invalid mode!\n");
}

static paddr_t get_pool_top(u8 mode)
{
	paddr_t partition_start;
	paddr_t pool_start;
	usize	used_page_count = get_used_page_count(mode);
	paddr_t pool_top;

	if (mode == SMODE_POOL) {
		partition_start = get_emod_manager_pa_start();
		pool_start		= partition_start + smode_pool_desc.offset;
	} else if (mode == UMODE_POOL) {
		partition_start = get_payload_pa_start();
		pool_start		= partition_start + umode_pool_desc.offset;
	} else {
		panic("Invalid mode!\n");
	}

	pool_top = pool_start + used_page_count * PAGE_SIZE;
	return pool_top;
}

static paddr_t alloc_page(usize number_of_pages, u8 mode)
{
	paddr_t pool_top = get_pool_top(mode);
	usize	used_number_of_pages;
	usize	expected_number_of_pages;
	usize	pool_size;

	if (mode == SMODE_POOL)
		pool_size = smode_pool_desc.size;
	else if (mode == UMODE_POOL)
		pool_size = umode_pool_desc.size;
	else
		panic("Invalid mode!\n");

	used_number_of_pages 		= get_used_page_count(mode);
	expected_number_of_pages	= used_number_of_pages + number_of_pages;

	if (expected_number_of_pages * PAGE_SIZE > pool_size) {
		show(number_of_pages);
		show(mode);
		panic("Pool out of pages!\n");
	}

	show(mode);
	debug("allocating %lu pages\n", number_of_pages);
	debug("%lu pages used in total, %lu pages left\n",
		expected_number_of_pages,
		(pool_size >> PAGE_SHIFT) - expected_number_of_pages);
	show(pool_top);

	set_used_page_count(expected_number_of_pages, mode);

	return pool_top;
}

paddr_t alloc_smode_page(usize number_of_pages)
{
	return alloc_page(number_of_pages, SMODE_POOL);
}

paddr_t alloc_umode_page(usize number_of_pages)
{
	return alloc_page(number_of_pages, UMODE_POOL);
}

usize get_smode_page_pool_avail_size()
{
	return smode_pool_desc.size
		- smode_pool_desc.used_page_count * PAGE_SIZE;
}

usize get_umode_page_pool_avail_size()
{
	return umode_pool_desc.size
		- umode_pool_desc.used_page_count * PAGE_SIZE;
}

// invoked only once
void map_smode_page_pool()
{
	paddr_t start_pa = get_emod_manager_pa_start() + smode_pool_desc.offset;
	usize pool_size = smode_pool_desc.size;

	for (int i = 0; i < (pool_size >> PAGE_SHIFT); i++) {
		map_page(
			start_pa + i * PAGE_SIZE + get_va_pa_offset(),
			start_pa + i * PAGE_SIZE,
			PTE_R | PTE_W,
			SV39_LEVEL_PAGE
		);
	}
}