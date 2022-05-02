#include "memory_pool.h"
#include "dependency.h"
#include <enclave/enclave_ops.h>
#include <memory/page_table.h>
#include <memory/memory.h>

#define POOL_VA_START 0xC00000000; // 0xC_0000_0000

static paddr_t 	pool_brk_pa;
static vaddr_t 	pool_brk_va = POOL_VA_START;
static usize	used_size = 0;
static const usize number_of_partitions = 5000UL;
static const usize total_size = PARTITION_SIZE * number_of_partitions;

void init_memory_pool()
{
	paddr_t pool_brk_pa = __ecall_ebi_mem_alloc(number_of_partitions);
	show(pool_brk_pa);

	for (int i = 0; i < number_of_partitions; i++) {
		map_page(
			pool_brk_va + i * PARTITION_SIZE,
			pool_brk_pa + i * PARTITION_SIZE,
			PTE_R | PTE_W | PTE_X | PTE_U,
			SV39_LEVEL_MEGA
		);
	}

	show(pool_brk_va);
	show(pool_brk_pa);
}

// change this to kmalloc
// and provide a real malloc
void *malloc(usize size)
{
	usize expected_used_size = used_size + size;

	debug("[emodule malloc] size = 0x%lx\n", size);

	if (expected_used_size > total_size) {
		show(expected_used_size);
		show(total_size);
		panic("Pool out of page!\n");
	}

	vaddr_t ret = pool_brk_va;

	pool_brk_va += size;
	pool_brk_pa += size;

	return (void *)ret;
}

void *kmalloc(usize size)
{
	return malloc(size);
}

static void *memset(void *s, int c, usize count)
{
	char *temp = s;

	while (count > 0) {
		count--;
		*temp++ = c;
	}

	return s;
}

void *calloc(usize number, usize size)
{
	void *ret = malloc(number * size);
	memset(ret, 0, number * size);
	return ret;
}

void free(void *ptr)
{
	return;
}