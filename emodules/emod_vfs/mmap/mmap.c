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
	vaddr_t smode_va;
	struct mmap_addr *next;
};

static struct mmap_addr *mmap_addr;

static void *map_memalign(usize size, usize align, vaddr_t *smode_va)
{
	static vaddr_t cur_va = MMAP_START_VA;

	void *smode_addr = memalign(size, align);
	paddr_t pa = get_pa((vaddr_t)smode_addr);
	show(smode_addr);
	show(pa);

	u8 level = (align >= PARTITION_SIZE) ? SV39_LEVEL_MEGA : SV39_LEVEL_PAGE;
	usize page_size = (align >= PARTITION_SIZE) ? PARTITION_SIZE : PAGE_SIZE;

	vaddr_t map_va_start;
	paddr_t map_pa_start, map_pa_end;

	map_pa_start = ROUNDDOWN(pa, page_size);
	map_pa_end = ROUNDUP(pa + size, page_size);
	usize map_size = map_pa_end - map_pa_start;
	usize nr_page = map_size / page_size;

	map_va_start = ROUNDUP(cur_va, page_size);
	void *ret = (void *)map_va_start;

	for (int i = 0; i < nr_page; i++) {
		show(map_va_start + i * page_size);
		show(map_pa_start + i * page_size);
		map_page(
			map_va_start + i * page_size,
			map_pa_start + i * page_size,
			PTE_R | PTE_W | PTE_X | PTE_U,
			level
		);
	}

	cur_va += map_size + PARTITION_SIZE;
	*smode_va = (vaddr_t)smode_addr;

	return ret;
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

	vaddr_t new_smode_va;
	void *mem = map_memalign(len, PAGE_SIZE, &new_smode_va);
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
	// always zero
	// memset(mem, 0, len);

	new->begin = mem;
	new->end = mem + len;
	new->next = NULL;
	new->smode_va = new_smode_va;

	if (!mmap_addr)
		mmap_addr = new;
	else
		last->next = new;

	//DEBUG("addr = 0x%lx, len = 0x%lx\n", mem, len);
	
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

	//DEBUG("addr = 0x%lx, len = 0x%lx\n", addr, len);

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

			void *smode_addr = (void *)tmp->smode_va;

			free(tmp);
			free(smode_addr);
			// mmap_free(addr);

			//DEBUG("actually freed\n");

			return 0;
		}

		tmp = tmp->next;
	}

	/* No matching region found. But it is ok anyway */
	return 0;
}