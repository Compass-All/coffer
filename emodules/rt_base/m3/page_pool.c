#include "page_pool.h"
#include "../rt_console.h"
#include "../rt_ecall.h"
#include "page_table.h"

#define get_pa(va) (read_csr(satp) ? usr_get_pa(va) : (va - enc_va_pa_offset));

page_list_t page_pools[NUM_POOL];
uintptr_t va_top;

static uintptr_t malloc_from_sm(page_list_t* pool);

// This function will be used both before and after MMU gets turned on
static void page_pool_put_pa(uintptr_t pa, page_list_t* pool)
{
    uintptr_t prev;
    uintptr_t section_offset = pa - PARTITION_DOWN(pa);
    uintptr_t va = va_top + section_offset;
    uintptr_t accessible_va = va - VA_PA_OFFSET_NOMMU();

    if (!LIST_EMPTY(pool)) {
        prev = pool->tail - VA_PA_OFFSET_NOMMU();
        NEXT_PAGE(prev) = va;
    } else {
        em_debug("List empty, head set to 0x%lx, va_top: 0x%lx\n", va,
            va_top);
        pool->head = va;
    }

    NEXT_PAGE(accessible_va) = 0;
    pool->tail = va;
    pool->count++;
}

// Returns VA of the page, -1 on failure
// If out of memory, try to allocate memory partition(s) from SM
static uintptr_t page_pool_get_va(page_list_t* pool)
{
    uintptr_t page, va, next;
    if (LIST_EMPTY(pool)) {
        em_debug("Pool tail = 0x%lx\n", pool->tail);
        va = malloc_from_sm(pool);
        if (!va) {
            em_error("Ret=%d\n", va);
            return -1;
        }
    }
    page = pool->head - VA_PA_OFFSET_NOMMU();
    va = pool->head;
    next = NEXT_PAGE(page);

    pool->head = next;
    pool->count--;

    return va;
}

static uintptr_t malloc_from_sm(page_list_t* pool)
{
    uintptr_t addr, size;

    // TODO alloc multiple partitions
    ecall_mem_alloc(va_top);
    asm volatile("mv %0, a1"
                 : "=r"(addr));
    asm volatile("mv %0, a2"
                 : "=r"(size));
    em_debug("mem alloc result: allocated section pa: 0x%lx, size: 0x%lx\n",
        addr, size);
    if (!addr) {
        em_error("addr is NULL\n");
        return 0;
    }

    // linearly map the allocated memory by VA/PA offset
    em_debug("va_top = 0x%lx\n", va_top);
    map_page(va_top, addr, size >> EPAGE_SHIFT, PTE_V | PTE_W | PTE_R, 1);

    // put the allocated memory into mem pool
    for (uintptr_t page = addr; page < addr + size; page += EPAGE_SIZE) {
        page_pool_put_pa(page, pool);
    }

    // if (LIST_EMPTY(pool))
    //     em_error("Pool is empty?\n");

    va_top += EMEM_SIZE;

    return addr;
}

void page_pool_init(uintptr_t pool_addr, size_t pool_size, int idx)
{
    em_debug("Initializing page pool %d\n", (int)idx);

    uintptr_t pa;
    page_list_t* pool = &page_pools[idx];
    LIST_INIT(pool);
    for (pa = pool_addr; pa < pool_addr + pool_size; pa += EPAGE_SIZE) {
        page_pool_put_pa(pa, pool);
    }
}

uintptr_t page_pool_get_pa(int idx)
{
    uintptr_t va = page_pool_get_va(&page_pools[idx]);
    if (va == -1)
        return -1;
    return get_pa(va);
}