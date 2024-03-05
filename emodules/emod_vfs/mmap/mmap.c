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
	struct mmap_addr *next;
};

struct free_mega_page {
	struct free_mega_page *next;
	vaddr_t va;
};

struct free_page_block {
	struct free_page_block *next;

	vaddr_t va;
	usize count;
};

static struct mmap_addr *mmap_addr;
static struct free_page_block *free_page = NULL;
static struct free_mega_page *free_mega = NULL;

static vaddr_t mmap_top = 0x1000000000UL;

static void *alloc_small_page(usize num)
{
	if (num > PARTITION_SIZE / PAGE_SIZE) {
		printf(KRED "More pages than expected\n" RESET);
		panic("");
	}

	info("allocate %lu small pages\n", num);
	info("expected return value: 0x%lx\n", mmap_top);
	void *ret = (void *)mmap_top;

	while (num) {
		debug("%lu pages remained to be allocated\n", num);
		if (!free_page) {
			info("no free pages in list, allocating from m mode\n");
			paddr_t pa = __ecall_ebi_mem_alloc(1, NULL);
			usize left = PARTITION_SIZE / PAGE_SIZE - num;
			vaddr_t va = mmap_top;

			info("%lu more pages\n", left);

			for (int i = 0; i < PARTITION_SIZE / PAGE_SIZE; i++) {
				// debug("mapping page: va = 0x%lx, pa = 0x%lx, size = 0x%lx\n",
				// 		mmap_top, pa, 0x1000);
				map_page(mmap_top, pa, PTE_R | PTE_W | PTE_U, SV39_LEVEL_PAGE);
				mmap_top += PAGE_SIZE;
				pa += PAGE_SIZE;
			}

			if (left) {
				struct free_page_block *new =
					(void *)kmalloc(sizeof(struct free_page_block));
				new->count = left;
				new->va = (vaddr_t)(va + num * PAGE_SIZE);
				new->next = NULL;
				free_page = new;

				info("adding left pages to list, count = %lu, va = 0x%lx\n",
					new->count, new->va);

				return ret;
			}
		}
		debug("using free_page @ %p, va = 0x%lx, count = %lu\n",
			free_page, free_page->va, free_page->count);
		usize n = (num >= free_page->count) ? free_page->count : num;
		for (int i = 0; i < n; i++) {
			paddr_t pa = get_pa(free_page->va + i * PAGE_SIZE);
			debug("mapping page: va = 0x%lx, pa = 0x%lx, size = 0x%lx\n",
						mmap_top, pa, 0x1000);
			map_page(mmap_top, pa, PTE_R | PTE_W | PTE_U, SV39_LEVEL_PAGE);
			memset((void *)mmap_top, 0, PAGE_SIZE);
			mmap_top += PAGE_SIZE;
			pa += PAGE_SIZE;
		}
		if (num >= free_page->count) {
			info("free_page @ %p used up\n", free_page);
			struct free_page_block *used = free_page;
			free_page = free_page->next;
			free(used);
		} else {
			free_page->count -= num;
			free_page->va += n * PAGE_SIZE;
			debug("updating free_page: new va: 0x%lx, new count = %lu\n",
				free_page->va, free_page->count);
			return ret;
		}
		
		num -= n;
	}

	return ret;
}

static void *alloc_mega_page(usize num)
{
	mmap_top = PARTITION_UP(mmap_top);

	info("allocate %lu mega pages\n", num);
	info("expected return value: 0x%lx\n", mmap_top);
	void *ret = (void *)mmap_top;
	while (num) {
		debug("%lu mega pages remained to be allocated\n", num);
		paddr_t pa;
		if (!free_mega) {
			debug("no free mega pages left, allocating from mmode\n");
			usize left = num;
			while (left) {
				usize sug = left, allocated;
				do {
					allocated = sug;
					pa = __ecall_ebi_mem_alloc(allocated, &sug);
				} while (pa == -1UL);
				debug("allocated: 0x%lx, sug = 0x%lx\n", 
					allocated, sug);
				for (int i = 0; i < allocated; i++) {
					debug("mapping page: va = 0x%lx, pa = 0x%lx, size = 0x%lx\n",
						mmap_top, pa, 0x200000);
					map_page(mmap_top, pa, PTE_R | PTE_W | PTE_U, SV39_LEVEL_MEGA);
					mmap_top += PARTITION_SIZE;
					pa += PARTITION_SIZE;
				}
				left -=allocated;
			}
			return ret;
		}

		debug("using free_mega_page @ %p\n", free_mega);
		pa = get_pa(free_mega->va);
		debug("mega page pa = 0x%lx\n", pa);
		debug("mapping page: va = 0x%lx, pa = 0x%lx, size = 0x%lx\n",
						mmap_top, pa, 0x200000);
		map_page(mmap_top, pa, 0, SV39_LEVEL_MEGA);
		memset((void *)mmap_top, 0, PARTITION_SIZE);
		mmap_top += PARTITION_SIZE;
		struct free_mega_page *used = free_mega;
		free_mega = free_mega->next;
		free(used);
		num--;
	}

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

	void *mem;

	if (len >= PARTITION_SIZE) {
		mem = alloc_mega_page(PARTITION_UP(len) / PARTITION_SIZE);
	} else {
		mem = alloc_small_page(PAGE_UP(len) / PAGE_SIZE);
	}

	new = kmalloc(sizeof(struct mmap_addr));
	if (!new) {
		free(mem);
		errno = ENOMEM;
		return (void *) -1;
	}
	show(new);

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

			if (len >= PARTITION_SIZE) {
				struct free_mega_page *new = (void *)kmalloc(sizeof(struct free_mega_page));
				new->next = free_mega;
				new->va = (vaddr_t)addr;
				free_mega = new;
			} else {
				usize num = len / PAGE_SIZE;
				struct free_page_block *new = (void *)kmalloc(sizeof(struct free_page_block));
				new->count = num;
				new->va = (vaddr_t)addr;
				new->next = free_page;
				free_page = new;
			}
			free(tmp);

			//DEBUG("actually freed\n");

			return 0;
		}

		tmp = tmp->next;
	}

	/* No matching region found. But it is ok anyway */
	return 0;
}