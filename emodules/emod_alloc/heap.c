#include "heap.h"
#include "llist.h"
#include <types.h>
#include "dependency.h"
#include <enclave/enclave_ops.h>
#include <memory/page_table.h>
#include <memory/memory.h>

static uint overhead = sizeof(footer_t) + sizeof(node_t);

static heap_t heap;
static bin_t bins[BIN_COUNT];

// check whether the allocated memory is zeroed
__unused static int empty_check(void *p, size_t size)
{
    int ret = 0;
    char *ptr = (char *)p;
    info("empty check for allocated memory %p, size: 0x%lx\n", p, size);
    for (size_t i = 0; i < size; i++) {
        if (ptr[i]) {
            printd(KRED "%p (offset: 0x%lx): 0x%x\n" RESET,
                &ptr[i], i, ptr[i]);
            ret = 1;
        }
    }
    info("end of empty check\n");
    return ret;
}

__unused void display_chunk(node_t *head)
{
    printd(
KBLU
"struct head @ %p (ptr: %p)\n"
"\thole: (uint) %u\n"
"\tsize: (size_t) 0x%lx\n"
"\tnext: (node_t *) %p%s\n"
"\tprev: (node_t *) %p%s\n"
RESET,
    head, (void *)head + sizeof(node_t),
    head->hole, head->size,
    head->next, head->hole ? "" : " (meaningless)",
    head->prev, head->hole ? "" : " (meaningless)");

    footer_t *foot = get_foot(head);
    printd(
KBLU
"struct footer @ %p\n"
"\theader: (node_t *) %p\n"
RESET,
    foot,
    foot->header
    );
}

size_t get_index_by_bin(bin_t *bin)
{
    size_t offset = (usize)bin - (usize)&bins[0];
    return offset / sizeof(bin_t);
}

static void add_node_to_bin(node_t *node)
{
    if (!node || !node->hole || !node->size)
        panic("error adding node\n");

    add_node(heap.bins[get_bin_index(node->size)], node);
}

__unused void dump_bin(bin_t *bin)
{
    if (bin->head == NULL)
        return;

    size_t index = get_index_by_bin(bin);
    debug("bin %lu (~0x%lx):\n", index, 4UL << index);

    node_t *temp = bin->head;
    while (temp != NULL) {
        display_chunk(temp);
        temp = temp->next;
    }
}

__unused void dump_heap(bin_t *first_bin)
{
    printd(KRED "----- start heap dump\n" RESET);
    for (int i = 0; i < BIN_COUNT; i++) {
        dump_bin(&first_bin[i]);
    }
    printd(KRED "----- end heap dump\n" RESET);
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

void init_heap() {
    usize left = HEAP_INIT_PARTITION_NUM;

    vaddr_t start_va = POOL_VA_START;
    while (left > 0) {
        vaddr_t va = start_va;
        usize sug = left, allocated;
        paddr_t pa;

        do {
            allocated = sug;
            pa = __ecall_ebi_mem_alloc(allocated, &sug);
        } while (pa == -1UL);
        for (int i = 0; i < allocated; i++) {
    	    map_page(
    	    	va + i * PARTITION_SIZE,
    	        pa + i * PARTITION_SIZE,
    	    	PTE_R | PTE_W | PTE_X,
    	    	SV39_LEVEL_MEGA
    	    );
        }

		left -= allocated;
        va += allocated * PARTITION_SIZE;
    }

    for (int i = 0; i < BIN_COUNT; i++) {
        heap.bins[i] = &bins[i];
        debug("bin %d @ %p\n", i, &bins[i]);
    }

    // create header
    node_t *init_region = (node_t *)start_va;
    init_region->hole = 1;
    init_region->size = (HEAP_INIT_SIZE) - overhead;

    // create footer    
    create_foot(init_region);

    add_node_to_bin(init_region);

    heap.start = start_va;
    heap.end   = (start_va + HEAP_INIT_SIZE);

    show(heap.start);
    show(heap.end);
    show(get_wilderness());
}

static node_t *search_fit_node(size_t size, uint *index_ptr)
{
    uint index = get_bin_index(size);

    bin_t *temp = (bin_t *)heap.bins[index];
    node_t *found = get_best_fit(temp, size);

    while (found == NULL) {
        if (index + 1 >= BIN_COUNT) {
            info("fit node not found "
                "(size: 0x%lx, index = %u)\n",
                size, index);
            *index_ptr = -1;
            return NULL;
        }

        temp = heap.bins[++index];
        found = get_best_fit(temp, size);
    }

    *index_ptr = index;
    return found;
}

static size_t get_wild_size()
{
    node_t *wild = get_wilderness();
    if (!wild) {
        panic("NULL wild!\n");
    }
    return wild->size;
}

void *heap_alloc(size_t size) {
    debug("allocating size 0x%lx\n", size);

    uint index = 0;
    // search for a chunk of proper size
    node_t *found = search_fit_node(size, &index);
    if (!found) {
        info("not found, increase the wild\n");
        size_t increment = size + 0x10000
            - get_wild_size();
        expand(increment);
        found = search_fit_node(size, &index);
        if (!found)
            panic("cannot alloc\n");
    }
    // if the remaining size of the chunk is large enough,
    // split the chunk and add the remaining part to the
    // free bins
    if ((found->size - size) > (overhead + MIN_ALLOC_SZ)) {
        node_t *split = (node_t *) (((char *) found
            + sizeof(node_t) + sizeof(footer_t)) + size);
        split->size = found->size - size - overhead;
        split->hole = 1;

        create_foot(split);

        uint new_idx = get_bin_index(split->size);

        add_node(heap.bins[new_idx], split); 

        found->size = size; 
        create_foot(found);
    }

    found->hole = 0; 
    remove_node(heap.bins[index], found); 
    
    node_t *wild = get_wilderness();
    if (!wild)
        panic("wild == NULL\n");
    if (wild->size < MIN_WILDERNESS) {
        info("wild too small (0x%lx), try to expand\n", wild->size);
        uint success = expand(MIN_WILDERNESS - wild->size);
        if (success == 0) {
            panic("expand failed\n");
            return NULL;
        }
    }
    else if (wild->size > MAX_WILDERNESS) {
        contract(MAX_WILDERNESS - wild->size);
    }

    found->prev = NULL;
    found->next = NULL;

    debug("alloc return chunk: \n");
    display_chunk(found);
    dump_heap(&bins[0]);

    void *ret = (void *)((u64)found + sizeof(node_t));

    return ret; 
}

// todo: return the new region near the top boundary of the found node
void *heap_memalign(size_t sz, size_t align)
{
    debug("memalign size: 0x%lx, align: 0x%lx\n",
        sz, align);

    sz = ROUNDUP(sz, align);

    size_t size = ROUNDUP(sz + overhead, align) + align;

    uint index = 0;
    node_t *found = search_fit_node(size, &index);
    if (!found) {
        info("not found, increase the wild\n");
        size_t increment = size + 0x10000
            - get_wild_size();
        expand(increment);
        found = search_fit_node(size, &index);
        if (!found)
            panic("cannot alloc\n");
    }

    // remove found node
    remove_node(heap.bins[index], found);

    // setup the aligned node (the returned one)
    void *p = (void *)ROUNDUP((u64)found + sizeof(node_t), align);
    node_t *node = (node_t *)(p - sizeof(node_t));
    node->hole = 0;
    node->size = sz;
    create_foot(node);

    size_t dist = (u64)node - (u64)found;
    size_t prev_sz = dist - overhead;
    size_t remain_sz = found->size - prev_sz - sz - 2 * overhead;

    __unused int prev_new = 0, remain_new = 0;

    if (prev_sz > MIN_ALLOC_SZ) {
        found->hole = 1;
        found->size = prev_sz;
        create_foot(found);
        add_node_to_bin(found);

        prev_new = 1;
    }

    node_t *remain;
    if (remain_sz > MIN_ALLOC_SZ) {
        remain = (node_t *)((void *)node + sz + overhead);
        remain->hole = 1;
        remain->size = remain_sz;
        create_foot(remain);
        add_node_to_bin(remain);

        remain_new = 1;
    }

    node_t *wild = get_wilderness();
    if (!wild)
        panic("wild == NULL\n");
    if (wild->size < MIN_WILDERNESS) {
        info("wild too small (0x%lx), try to expand\n", wild->size); 
        uint success = expand(MIN_WILDERNESS - wild->size);
        if (success == 0) {
            panic("expand failed\n");
            return NULL;
        }
    }
    else if (wild->size > MAX_WILDERNESS) {
        contract(MAX_WILDERNESS - wild->size);
    }

    if (prev_new) {
        debug("previous new chunk:\n");
        display_chunk(found);
    } else {
        debug("previous chunk is too small\n");
    }

    debug("memalign return chunk:\n");
    display_chunk(node);

    if (remain_new) {
        debug("remain chunk:\n");
        display_chunk(remain);
    } else {
        debug("remaining chunk is too small\n");
    }

    dump_heap(&bins[0]);

    return p;
}

void *heap_calloc(size_t number, size_t size)
{
	void *ret = heap_alloc(number * size);
	memset(ret, 0, number * size);
	return ret;
}

void heap_free(void *p) {
    bin_t *list;
    footer_t *new_foot, *old_foot;

    node_t *head = (node_t *) ((char *) p - sizeof(node_t));
    debug("free chunk: \n");
    display_chunk(head);

    if (head == (node_t *)heap.start) {
        head->hole = 1; 
        add_node_to_bin(head);
        return;
    }

    node_t *next = (node_t *) ((char *) get_foot(head) + sizeof(footer_t));
    footer_t *f = (footer_t *) ((char *) head - sizeof(footer_t));
    node_t *prev = f->header;

    debug("prev chunk:\n");
    display_chunk(prev);
    debug("next chunk:\n");
    display_chunk(next);

    if (prev->hole) {
        list = heap.bins[get_bin_index(prev->size)];
        remove_node(list, prev);

        prev->size += overhead + head->size;
        new_foot = get_foot(head);
        new_foot->header = prev;

        head = prev;

        info("prev is free, merged chunk:\n");
        display_chunk(head);
    }

    if (next->hole) {
        list = heap.bins[get_bin_index(next->size)];
        remove_node(list, next);

        head->size += overhead + next->size;

        old_foot = get_foot(next);
        old_foot->header = 0;
        next->size = 0;
        next->hole = 0;
        
        new_foot = get_foot(head);
        new_foot->header = head;

        info("next is free, merged chunk:\n");
        display_chunk(head);
    }

    head->hole = 1;
    add_node(heap.bins[get_bin_index(head->size)], head);

    dump_heap(&bins[0]);
}

uint expand(size_t sz)
{
	usize nr_part = (PARTITION_UP(heap.end + sz) - PARTITION_UP(heap.end))
			>> PARTITION_SHIFT;
    usize left = nr_part;

	vaddr_t va = PARTITION_UP(heap.end);
	while (left > 0) {
        usize sug = left;
        usize allocated;
        paddr_t pa;

        do {
            allocated = sug;
            pa = __ecall_ebi_mem_alloc(allocated, &sug);
        } while (pa == -1UL);

		for (int i = 0; i < allocated; i++) {
            show(va + i * PARTITION_SIZE);
            show(pa + i * PARTITION_SIZE);
			map_page(
				va + i * PARTITION_SIZE,
				pa + i * PARTITION_SIZE,
				PTE_R | PTE_W | PTE_X,
				SV39_LEVEL_MEGA
			);
		}

        left -= allocated;
        va += allocated * PARTITION_SIZE;
	}

    node_t *wild = get_wilderness();
    footer_t *wild_foot = get_foot(wild);
    memset((void *)wild_foot, 0, sizeof(footer_t));

    remove_node(heap.bins[get_bin_index(wild->size)], wild);
    wild->size += sz;
	heap.end += sz;

    create_foot(wild);
    add_node(heap.bins[get_bin_index(wild->size)], wild);

    info("expand heap by 0x%lx\n", sz);
    show(heap.start);
    show(heap.end);

    debug("wild chunk after expansion:\n");
    display_chunk(wild);

    return 1;
}

void contract(size_t sz) {
    return;
}

uint get_bin_index(size_t sz) {
    uint index = 0;
    sz = sz < 4 ? 4 : sz;

    while (sz >>= 1) index++; 
    index -= 2;
    if (index > BIN_MAX_IDX) index = BIN_MAX_IDX; 
    return index;
}

void create_foot(node_t *head) {
    footer_t *foot = get_foot(head);
    foot->header = head;
}

footer_t *get_foot(node_t *node) {
    return (footer_t *) ((char *) node + sizeof(node_t) + node->size);
}

node_t *get_wilderness() {
    footer_t *wild_foot = (footer_t *) ((char *) heap.end - sizeof(footer_t));
    return wild_foot->header;
}
