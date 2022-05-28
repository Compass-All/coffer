#include "../dependency.h"
#include "mman.h"
#include "../vfs/errno.h"
#include <types.h>
#include <memory/memory.h>
#include <string.h>
#include "../vfs/stat.h"
#include "enclave/enclave_ops.h"
#include "../dependency.h"

struct mmap_addr {
	void *begin;
	void *end;
	struct mmap_addr *next;
};

static struct mmap_addr *mmap_addr;

static void *alloc_from_mmode(usize size)
{
	static vaddr_t current_va = MMAP_START_VA;
	void *ret = (void *)current_va;

	vaddr_t cur_aligned = PARTITION_UP(current_va);
	vaddr_t tar_aligned = PARTITION_UP(current_va + size);
	usize diff = tar_aligned - cur_aligned;
	usize nr_part = diff >> PARTITION_SHIFT;

	show(current_va);
	show(size);
	show(nr_part);

	if (nr_part > 0) {
		paddr_t pa = __ecall_ebi_mem_alloc(nr_part);
		vaddr_t va = cur_aligned;
		for (int i = 0; i < nr_part; i++) {
			show(va + i * PARTITION_SIZE);
			show(pa + i * PARTITION_SIZE);
			map_page(
				va + i * PARTITION_SIZE, 
				pa + i * PARTITION_SIZE,
				PTE_R | PTE_W | PTE_X | PTE_U,
				SV39_LEVEL_MEGA
			);
		}
	}

	current_va += size;

	return ret;
}

static void *memalign(usize align, usize size)
{
	usize alloc_size = ROUNDUP(size, align);
	return alloc_from_mmode(alloc_size);
}

#define mmap_free(_)

void *mmap(
	void	*addr,
	size_t	len,
	int		prot,
	int		flags,
	int		fildes,
	off_t	off
)
{
	struct mmap_addr *tmp = mmap_addr, *last = NULL, *new = NULL;

	show(addr);
	show(len);
	show(prot);
	show(flags);
	show(fildes);
	show(off);

	if (!len) {
		errno = EINVAL;
		return (void *) -1;
	}

	/* Check if parameters match the ones that go use
	 * Otherwise return 0 (unimplemented mmap)
	 */
	if (fildes != -1 || off)
		return 0;
	if (!(prot & (PROT_READ|PROT_WRITE)) && (prot != 0))
		return 0;
	if (!(flags & (MAP_ANON|MAP_PRIVATE)) &&
			!(flags & (MAP_FIXED|MAP_ANON|MAP_PRIVATE)) &&
			!(flags & (MAP_NORESERVE|MAP_ANON|MAP_PRIVATE)))
		return 0;

	while (tmp) {
		if (addr) {
			if (addr >= tmp->begin && addr < tmp->end)
				return addr;
		}
		last = tmp;
		tmp = tmp->next;
	}

	void *mem = memalign(PAGE_SIZE, len);
	if (!mem) {
		errno = ENOMEM;
		return (void *) -1;
	}

	new = kmalloc(sizeof(struct mmap_addr));
	if (!new) {
		mmap_free(mem);
		errno = ENOMEM;
		return (void *) -1;
	}

	show(mem);
	show(new);

	debug("len = 0x%x\n", len);
	/* The caller expects the memory to be zeroed */
	memset(mem, 0, len);

	new->begin = mem;
	new->end = mem + len;
	new->next = NULL;

	if (!mmap_addr)
		mmap_addr = new;
	else
		last->next = new;
	
	return mem;
}

int munmap(void* addr, size_t len)
{
	struct mmap_addr *tmp = mmap_addr, *prev = NULL;

	show(addr);
	show(len);

	if (!len) {
		errno = EINVAL;
		return -1;
	}

	if (!addr)
		return 0;

	while (tmp) {
		if (addr >= tmp->begin && addr < tmp->end) {
			/* We cannot release only some part of the allocation.
			 * In that case, pretend we have done it and hope
			 * everything will be fine
			 */
			if (len != (__uptr)tmp->end - (__uptr)tmp->begin)
				return 0;

			/* Caller wants to unmap the whole region. Easy! */
			if (!prev)
				mmap_addr = tmp->next;
			else
				prev->next = tmp->next;

			free(tmp);

			mmap_free(addr);
			return 0;
		}

		tmp = tmp->next;
	}

	/* No matching region found. But it is ok anyway */
	return 0;
}