#include "../dependency.h"
#include "memory/page_table.h"
#include "mman.h"
#include "../vfs/errno.h"
#include <types.h>
#include <memory/memory.h>
#include <string.h>
#include "../vfs/stat.h"
#include "enclave/enclave_ops.h"
#include "../dependency.h"
#include "util/console.h"

struct mmap_addr {
	void *begin;
	void *end;
	void *smode_va;
	u8	 level;
	struct mmap_addr *next;
};

static struct mmap_addr *mmap_addr;

// map the pages allocated with memalign with umode access
// smode_va and size is expected to be page aliged
static void *map_umode(void *smode_va, usize size, u8 level)
{
	static vaddr_t mmap_va_ptr = MMAP_START_VA;
	void *ret = (void *)mmap_va_ptr;
	usize align_size = level == SV39_LEVEL_MEGA ? PARTITION_SIZE : PAGE_SIZE;


	for (int i = 0; i < ROUNDUP(size, align_size) / align_size; i++) {
		vaddr_t va = mmap_va_ptr + i * align_size;
		paddr_t pa = get_pa((vaddr_t)smode_va + i * align_size);
		show(va);
		show(pa);
		show(align_size);
   	    map_page(va, pa, PTE_R | PTE_W | PTE_U, level);
	}

	mmap_va_ptr = PARTITION_UP(mmap_va_ptr + size + PARTITION_SIZE);

	return ret;
}

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

	u8 level;
	usize align_size;
	if (len >= PARTITION_SIZE) {
		level = SV39_LEVEL_MEGA;
		align_size = PARTITION_SIZE;
	} else {
		level = SV39_LEVEL_PAGE;
		align_size = PAGE_SIZE;
	}
	void *mem = memalign(len, align_size);
	if (!mem) {
		errno = ENOMEM;
		return (void *) -1;
	}
	show(mem);
	/* The caller expects the memory to be zeroed */
	memset(mem, 0, ROUNDUP(len, align_size));

	new = kmalloc(sizeof(struct mmap_addr));
	if (!new) {
		free(mem);
		errno = ENOMEM;
		return (void *) -1;
	}
	show(new);

	void *umode_va = map_umode(mem, ROUNDUP(len, align_size), level);

	new->begin = umode_va;
	new->end = umode_va + len;
	new->next = NULL;
	new->smode_va = mem;
	new->level = level;

	if (!mmap_addr)
		mmap_addr = new;
	else
		last->next = new;

	return umode_va;
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

			u8 level = tmp->level;
			usize align_size = level == SV39_LEVEL_MEGA ? PARTITION_SIZE : PAGE_SIZE;
			// unmap umode page here
			for (int i = 0; i < len / align_size; i++)
				unmap_page((vaddr_t)(addr + i * align_size), level);
			free(tmp->smode_va);
			free(tmp);

			//DEBUG("actually freed\n");

			return 0;
		}

		tmp = tmp->next;
	}

	/* No matching region found. But it is ok anyway */
	return 0;
}