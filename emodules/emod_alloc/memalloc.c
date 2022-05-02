#include "memalloc.h"
#include <types.h>
#include "dependency.h"
#include <enclave/enclave_ops.h>
#include <memory/page_table.h>
#include <memory/memory.h>

typedef char ALIGN[16];

union header {
	struct {
		size_t size;
		unsigned is_free;
		union header *next;
	} s;
	/* force the header to be aligned to 16 bytes */
	ALIGN stub;
};
typedef union header header_t;

header_t *head = NULL, *tail = NULL;

void print_mem_list();

#define POOL_VA_START 0xC00000000; // 0xC_0000_0000

static vaddr_t brk = POOL_VA_START;

static void *sbrk(s64 increment)
{
	if (increment == 0)
		return (void *)brk;

	if (increment < 0) {
		u64 old_brk = brk;
		brk += increment;

		debug("decreasing brk\n");
		show(increment);
		show(old_brk);
		show(brk);

		return (void *)old_brk;
	}

	static usize count = 0;

	vaddr_t old_brk = brk;
	vaddr_t new_brk = brk + increment;

	usize number_of_partitions = (PARTITION_UP(new_brk) - PARTITION_UP(brk))
		>> PARTITION_SHIFT;

	if (number_of_partitions) {
		paddr_t pa = __ecall_ebi_mem_alloc(number_of_partitions);

		show(pa);
		show(PARTITION_UP(brk));
		show(increment);
		show(number_of_partitions);

		for (int i = 0; i < number_of_partitions; i++) {
			map_page(
				PARTITION_UP(brk) 	+ i * PARTITION_SIZE,
				pa 					+ i * PARTITION_SIZE,
				PTE_R | PTE_W | PTE_X | PTE_U,
				SV39_LEVEL_MEGA
			);
		}

		count += number_of_partitions;
		show(count);
	}

	brk = new_brk;

	return (void *)old_brk;
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

static void *memcpy(void *dest, const void *src, size_t count)
{
	char *temp1	  = dest;
	const char *temp2 = src;

	while (count > 0) {
		*temp1++ = *temp2++;
		count--;
	}

	return dest;
}

header_t *get_free_block(size_t size)
{
	header_t *curr = head;
	while(curr) {
		/* see if there's a free block that can accomodate requested size */
		if (curr->s.is_free && curr->s.size >= size)
			return curr;
		curr = curr->s.next;
	}
	return NULL;
}

void free(void *block)
{
	header_t *header, *tmp;
	/* program break is the end of the process's data segment */
	void *programbreak;

	show(block);

	if (!block)
		return;
	header = (header_t*)block - 1;
	/* sbrk(0) gives the current program break address */
	programbreak = sbrk(0);

	/*
	   Check if the block to be freed is the last one in the
	   linked list. If it is, then we could shrink the size of the
	   heap and release memory to OS. Else, we will keep the block
	   but mark it as free.
	 */
	if ((char*)block + header->s.size == programbreak) {
		if (head == tail) {
			head = tail = NULL;
		} else {
			tmp = head;
			while (tmp) {
				if(tmp->s.next == tail) {
					tmp->s.next = NULL;
					tail = tmp;
				}
				tmp = tmp->s.next;
			}
		}
		/*
		   sbrk() with a negative argument decrements the program break.
		   So memory is released by the program to OS.
		*/
		sbrk(0 - header->s.size - sizeof(header_t));
		/* Note: This lock does not really assure thread
		   safety, because sbrk() itself is not really
		   thread safe. Suppose there occurs a foregin sbrk(N)
		   after we find the program break and before we decrement
		   it, then we end up realeasing the memory obtained by
		   the foreign sbrk().
		*/
		return;
	}
	header->s.is_free = 1;
}

void *kmalloc(size_t size)
{
	return malloc(size);
}

void *malloc(size_t size)
{
	show(size);

	size_t total_size;
	void *block;
	header_t *header;
	if (!size)
		return NULL;
	header = get_free_block(size);
	if (header) {
		/* Woah, found a free block to accomodate requested memory. */
		header->s.is_free = 0;
		return (void*)(header + 1);
	}
	/* We need to get memory to fit in the requested block and header from OS. */
	total_size = sizeof(header_t) + size;
	block = sbrk(total_size);
	if (block == (void*) -1) {
		return NULL;
	}
	header = block;
	header->s.size = size;
	header->s.is_free = 0;
	header->s.next = NULL;
	if (!head)
		head = header;
	if (tail)
		tail->s.next = header;
	tail = header;
	return (void*)(header + 1);
}

void *calloc(size_t num, size_t nsize)
{
	size_t size;
	void *block;
	if (!num || !nsize)
		return NULL;
	size = num * nsize;
	/* check mul overflow */
	if (nsize != size / num)
		return NULL;
	block = malloc(size);
	if (!block)
		return NULL;
	memset(block, 0, size);
	return block;
}

void *realloc(void *block, size_t size)
{
	header_t *header;
	void *ret;
	if (!block || !size)
		return malloc(size);
	header = (header_t*)block - 1;
	if (header->s.size >= size)
		return block;
	ret = malloc(size);
	if (ret) {
		/* Relocate contents to the new bigger block */
		memcpy(ret, block, header->s.size);
		/* Free the old memory block */
		free(block);
	}
	return ret;
}

/* A debug function to print the entire link list */
void print_mem_list()
{
	header_t *curr = head;
	debug("head = %p, tail = %p \n", (void*)head, (void*)tail);
	while(curr) {
		debug("addr = %p, size = %zu, is_free=%u, next=%p\n",
			(void*)curr, curr->s.size, curr->s.is_free, (void*)curr->s.next);
		curr = curr->s.next;
	}
}
