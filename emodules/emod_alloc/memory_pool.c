#include "memory_pool.h"
#include "dependency.h"
#include <enclave/enclave_ops.h>
#include <memory/page_table.h>

#define POOL_VA_START 0xC00000000; // 0xC_0000_0000

static paddr_t 	pool_brk_pa;
static vaddr_t 	pool_brk_va = POOL_VA_START;
static usize	used_size = 0;
static const usize total_size = 0x200000UL;

void init_memory_pool()
{
	paddr_t pool_brk_pa = __ecall_ebi_mem_alloc(1UL);

	map_page(
		pool_brk_va,
		pool_brk_pa,
		PTE_R | PTE_W | PTE_X,
		SV39_LEVEL_MEGA
	);

	show(pool_brk_va);
	show(pool_brk_pa);
}

void *malloc(usize size)
{
	usize expected_used_size = used_size + size;

	if (expected_used_size > total_size)
		panic("Pool out of page!\n");

	vaddr_t ret = pool_brk_va;

	pool_brk_va += size;
	pool_brk_pa += size;

	return (void *)ret;
}