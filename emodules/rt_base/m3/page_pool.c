#include "page_pool.h"
#include "../rt_console.h"
#include "../rt_ecall.h"
#include "page_table.h"

page_list_t page_pools[NUM_POOL];
uintptr_t va_top;

// This function will be used both before and after MMU gets turned on
static void page_pool_put(page_list_t* pool, uintptr_t pa, uintptr_t pa_start, size_t n_pages)
{
    uintptr_t prev;
    uintptr_t offset = pa - pa_start;
    uintptr_t va = va_top + offset;
    uintptr_t accessible_va = MMU_ENABLED() ? va : pa;
    size_t i;

    for (i = 0; i < n_pages; ++i) {
        if (!LIST_EMPTY(pool)) {
            prev = pool->tail;
            if (!MMU_ENABLED()) {
                prev -= enc_va_pa_offset;
            }
            PAGE_NEXT(prev) = va;
        } else {
            em_debug("List empty, head set to 0x%lx, va_top: 0x%lx\n", va, va_top);
            pool->head = va;
        }
        // em_debug("pa = 0x%llx, va = 0x%llx, accessible_va = 0x%llx\n", pa, va, accessible_va);

        PAGE_NEXT(accessible_va) = 0;
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
size_t page_pool_get(int idx, uintptr_t* pva, size_t n_pages)
{
    uintptr_t page, next;
    size_t n_alloc, i;
    page_list_t* pool = &page_pools[idx];

    if (LIST_EMPTY(pool)) {
        em_debug("Pool tail = 0x%lx\n", pool->tail);
        if (!malloc_from_sm(pool, PARTITION_UP(n_pages << EPAGE_SHIFT) >> PARTITION_SHIFT)) {
            *pva = -1;
            return 0;
        }
    }

    if (pool->count < n_pages) {
        n_alloc = pool->count;
    } else {
        n_alloc = n_pages;
    }

    *pva = pool->head;
    pool->count -= n_alloc;
    for (i = 0; i < n_alloc; ++i) {
        page = pool->head;
        if (!MMU_ENABLED()) {
            page -= enc_va_pa_offset;
        }
        next = PAGE_NEXT(page);
        pool->head = next;
    }

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

// size_t page_pool_get_pa(int idx, uintptr_t* vpa, size_t n_pages)
// {
//     uintptr_t va;
//     size_t n_alloc = page_pool_get_va(&page_pools[idx], &va, n_pages);
//     *vpa = (va == -1) ? -1 : get_pa(va);
//     return n_alloc;
// }