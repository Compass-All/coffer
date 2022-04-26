#include "../dependency.h"
#include "mman.h"
#include "../vfs/errno.h"
#include <types.h>
#include <memory/memory.h>
#include <string.h>
#include "../vfs/stat.h"

struct mmap_addr {
	void *begin;
	void *end;
	struct mmap_addr *next;
};

static struct mmap_addr *mmap_addr;

static void *memalign(usize align, usize size)
{
	usize alloc_size = ROUNDUP(size, align);
	return malloc(alloc_size);
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

	void *mem = memalign(PAGE_SIZE, len);
	if (!mem) {
		errno = ENOMEM;
		return (void *) -1;
	}

	new = malloc(sizeof(struct mmap_addr));
	if (!new) {
		free(mem);
		errno = ENOMEM;
		return (void *) -1;
	}

	show(mem);
	show(new);

	debug("len = 0x%x\n", len);
	/* The caller expects the memory to be zeroed */
	memset(mem, 0, len);

	debug("CP0\n");

	new->begin = mem;
	new->end = mem + len;
	new->next = NULL;

	debug("CP1\n");
	if (!mmap_addr)
		mmap_addr = new;
	else
		last->next = new;
	
	debug("CP2\n");
	return mem;
}
