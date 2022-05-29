#include "heap.h"
#include "llist.h"
#include <types.h>
#include "dependency.h"
#include <enclave/enclave_ops.h>
#include <memory/page_table.h>
#include <memory/memory.h>

uint offset = 8;
static uint overhead = sizeof(footer_t) + sizeof(node_t);

static heap_t heap;
static bin_t bins[BIN_COUNT];

void init_heap() {
	paddr_t pa = __ecall_ebi_mem_alloc(HEAP_INIT_PARTITION_NUM);
	vaddr_t va = POOL_VA_START;
    for (int i = 0; i < HEAP_INIT_PARTITION_NUM; i++) {
	    map_page(
	    	va + i * PARTITION_SIZE,
	        pa + i * PARTITION_SIZE,
	    	PTE_R | PTE_W | PTE_X,
	    	SV39_LEVEL_MEGA
	    );
    }

    show(pa);
    show(va);

    for (int i = 0; i < BIN_COUNT; i++) {
        heap.bins[i] = &bins[i];
    }

    // create header
    node_t *init_region = (node_t *) va;
    init_region->hole = 1;
    init_region->size = (HEAP_INIT_SIZE) - sizeof(node_t) - sizeof(footer_t);

    // create footer    
    create_foot(init_region);

    add_node(heap.bins[get_bin_index(init_region->size)], init_region);

    heap.start = va;
    heap.end   = (va + HEAP_INIT_SIZE);

    node_t *wild = get_wilderness();
    show(wild);
    show(wild->size);

    show(0);
}

static node_t *search_fit_node(size_t size, uint *index_ptr)
{
    uint index = get_bin_index(size);

    bin_t *temp = (bin_t *)heap.bins[index];
    node_t *found = get_best_fit(temp, size);

    while (found == NULL) {
        if (index + 1 >= BIN_COUNT) {
            debug("not found\n");
            *index_ptr = -1;
            return NULL;
        }

        temp = heap.bins[++index];
        found = get_best_fit(temp, size);
    }

    show(found);
    show(index);
    *index_ptr = index;
    return found;
}

static size_t get_wild_size()
{
    node_t *wild = get_wilderness();
    show(wild);
    if (!wild) {
        show(heap.start);
        show(heap.end);
        panic("NULL wild!\n");
    }
    return wild->size;
}

void *heap_alloc(size_t size) {
    uint index = 0;
    node_t *found = search_fit_node(size, &index);
    if (!found) {
        debug("not found, increase the wild\n");
        size_t increment = size + 0x10000
            - get_wild_size();
        show(increment);
        expand(increment);
        found = search_fit_node(size, &index);
        if (!found)
            panic("cannot alloc\n");
    }

    show(found);
    if ((found->size - size) > (overhead + MIN_ALLOC_SZ)) {
        node_t *split = (node_t *) (((char *) found
            + sizeof(node_t) + sizeof(footer_t)) + size);
        split->size = found->size - size - sizeof(node_t) - sizeof(footer_t);
        split->hole = 1;

        show(split);
        show(split->size);
   
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

    show(wild);
    show(wild->size);
    show(&found->next);
    return &found->next; 
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

void *heap_calloc(size_t number, size_t size)
{
	void *ret = heap_alloc(number * size);
	memset(ret, 0, number * size);
	return ret;
}

void heap_free(void *p) {
    bin_t *list;
    footer_t *new_foot, *old_foot;

    debug("Check Point 1\n");

    node_t *head = (node_t *) ((char *) p - offset);
    if (head == (node_t *) (uintptr_t) heap.start) {
        debug("free heap start node\n");
        head->hole = 1; 
        add_node(heap.bins[get_bin_index(head->size)], head);
        return;
    }

    node_t *next = (node_t *) ((char *) get_foot(head) + sizeof(footer_t));
    footer_t *f = (footer_t *) ((char *) head - sizeof(footer_t));
    node_t *prev = f->header;
    
    show(next);
    show(f);
    show(prev);

    if (prev->hole) {
        debug("prev is free\n");

        list = heap.bins[get_bin_index(prev->size)];
        remove_node(list, prev);

        prev->size += overhead + head->size;
        new_foot = get_foot(head);
        new_foot->header = prev;

        head = prev;
    }

    if (next->hole) {
        debug("next is free\n");

        list = heap.bins[get_bin_index(next->size)];
        remove_node(list, next);

        head->size += overhead + next->size;

        old_foot = get_foot(next);
        old_foot->header = 0;
        next->size = 0;
        next->hole = 0;
        
        new_foot = get_foot(head);
        new_foot->header = head;
    }

    head->hole = 1;
    add_node(heap.bins[get_bin_index(head->size)], head);
    debug("Check Point 3\n");
}

uint expand(size_t sz) {

	usize nr_part = (PARTITION_UP(heap.end + sz) - PARTITION_UP(heap.end))
			>> PARTITION_SHIFT;

    show(sz);
    show(nr_part);
    show(heap.end);

    show(PARTITION_UP(heap.end + sz));
    show(PARTITION_UP(heap.end));

	if (nr_part > 0) {
		paddr_t pa = __ecall_ebi_mem_alloc(nr_part);
		vaddr_t va = PARTITION_UP(heap.end);

        show(pa);
        show(va);
        show(nr_part);

		for (int i = 0; i < nr_part; i++) {
			map_page(
				va + i * PARTITION_SIZE,
				pa + i * PARTITION_SIZE,
				PTE_R | PTE_W | PTE_X,
				SV39_LEVEL_MEGA
			);
		}
	}

    node_t *wild = get_wilderness();
    remove_node(heap.bins[get_bin_index(wild->size)], wild);
    show(wild);
    show(wild->size);

    wild->size += sz;

    debug("new wild size:\n");
    show(wild->size);
    create_foot(wild);
    add_node(heap.bins[get_bin_index(wild->size)], wild);

	heap.end += sz;

    show(wild);
    show(wild->size);

    return 1;
}

void contract(size_t sz) {
    return;
}

uint get_bin_index(size_t sz) {
    show(sz);

    uint index = 0;
    sz = sz < 4 ? 4 : sz;

    while (sz >>= 1) index++; 
    index -= 2;
    
    show(index);
    show(index > BIN_MAX_IDX);

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
