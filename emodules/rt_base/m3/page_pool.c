#include "page_pool.h"
#include "../rt_console.h"
#include "../rt_ecall.h"
#include "page_table.h"

#define get_pa(va) (read_csr(satp) ? usr_get_pa(va) : (va - enc_va_pa_offset));

page_list_t page_pools[NUM_POOL];
uintptr_t va_top;

// This function will be used both before and after MMU gets turned on
static void page_pool_put(page_list_t* pool, uintptr_t pa, uintptr_t pa_start, size_t n_pages)
{
    uintptr_t prev;
    uintptr_t offset = pa - pa_start;
    uintptr_t va = va_top + offset;
    // uintptr_t accessible_va = va - VA_PA_OFFSET_NOMMU();
    uintptr_t accessible_va;
    if (!read_csr(satp)) {
        accessible_va = pa;
    } else {
        accessible_va = va;
    }
    size_t i;

    em_debug("offset = 0x%llx\n", offset);
    em_debug("PARTITION_DOWN(pa) = 0x%llx, va_top = 0x%llx\n", PARTITION_DOWN(pa), va_top);
    em_debug("pa = 0x%llx, va = 0x%llx, accessible_va = 0x%llx\n", pa, va, accessible_va);
    for (i = 0; i < n_pages; ++i) {
        if (!LIST_EMPTY(pool)) {
            prev = pool->tail - VA_PA_OFFSET_NOMMU();
            NEXT_PAGE(prev) = va;
        } else {
            em_debug("List empty, head set to 0x%lx, va_top: 0x%lx\n", va,
                va_top);
            pool->head = va;
        }
        // em_debug("pa = 0x%llx, va = 0x%llx, accessible_va = 0x%llx\n", pa, va, accessible_va);

        NEXT_PAGE(accessible_va) = 0;
        pool->tail = va;
        pool->count++;

        pa += EPAGE_SIZE;
        va += EPAGE_SIZE;
        accessible_va += EPAGE_SIZE;
    }
}

static uintptr_t malloc_from_sm(page_list_t* pool, size_t n_partitions)
{
    uintptr_t addr, size;

    ecall_mem_alloc(va_top, n_partitions);
    asm volatile("mv %0, a1"
                 : "=r"(addr));
    asm volatile("mv %0, a2"
                 : "=r"(size));
    em_debug("mem alloc result: allocated section pa: 0x%llx, size: 0x%llx\n",
        addr, size);
    if (!addr) {
        em_error("addr is NULL\n");
        return 0;
    }

    // linearly map the allocated memory by VA/PA offset
    em_debug("va_top = 0x%lx\n", va_top);
    size = PAGE_UP(size);
    map_page(va_top, addr, size >> EPAGE_SHIFT, PTE_V | PTE_W | PTE_R, 1);

    // put the allocated memory into mem pool
    page_pool_put(pool, addr, addr, size >> EPAGE_SHIFT);
    va_top += size;

    return addr;
}

// Returns VA of the page, -1 on failure
// If out of memory, try to allocate memory partition(s) from SM
static size_t page_pool_get_va(page_list_t* pool, uintptr_t* pva, size_t n_pages)
{
    uintptr_t page, next;
    size_t n_alloc, i;

    em_debug("Checkpoint 0: pool->cnt = %d, n_pages = %d\n", pool->count, n_pages);
    if (LIST_EMPTY(pool)) {
        em_debug("Pool tail = 0x%lx\n", pool->tail);
        if (!malloc_from_sm(pool, PARTITION_UP(n_pages << EPAGE_SHIFT) >> PARTITION_SHIFT)) {
            *pva = -1;
            return 0;
        }
    }

    em_debug("Checkpoint 1\n");
    if (pool->count < n_pages) {
        n_alloc = pool->count;
    } else {
        n_alloc = n_pages;
    }

    em_debug("Checkpoint 2: pool->head = 0x%llx\n", pool->head);
    *pva = pool->head;
    pool->count -= n_alloc;
    em_debug("Checkpoint 3\n");
    for (i = 0; i < n_alloc; ++i) {
        page = pool->head - VA_PA_OFFSET_NOMMU();
        // em_debug("page = 0x%llx\n", page);
        next = NEXT_PAGE(page);
        pool->head = next;
    }
    em_debug("Checkpoint 4\n");

    return n_alloc;
}

void page_pool_init(uintptr_t pool_addr, uintptr_t pa_start, size_t pool_size, int idx)
{
    em_debug("Initializing page pool %d\n", (int)idx);

    // uintptr_t pa;
    page_list_t* pool = &page_pools[idx];
    LIST_INIT(pool);
    page_pool_put(pool, pool_addr, pa_start, pool_size >> EPAGE_SHIFT);
}

size_t page_pool_get_pa(int idx, uintptr_t* vpa, size_t n_pages)
{
    uintptr_t va;
    em_debug("Ding\n");
    size_t n_alloc = page_pool_get_va(&page_pools[idx], &va, n_pages);
    em_debug("Dong\n");
    *vpa = (va == -1) ? -1 : get_pa(va);
    em_debug("VA=0x%llx, PA=0x%llx\n", va, *vpa);
    return n_alloc;
}