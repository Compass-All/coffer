#include "inv_map.h"
#include "../rt_console.h"
#include "../rt_ecall.h"

inverse_map_t inv_map[INVERSE_MAP_ENTRY_NUM];
static uint16_t inv_map_head, inv_map_tail;

#define pairwise_less(x1, y1, x2, y2) (((x1) == (x2)) ? ((y1) < (y2)) : ((x1) < (x2)))
#define addr_shift(addr) ((addr) >> EPAGE_SHIFT)
#define addr_check(addr1, addr2, n) (addr_shift(addr1) + (n) == addr_shift(addr2))

#define INV_MAP_HEAD (inv_map[inv_map_head])
#define INVALID_IDX ((uint16_t)-1)

/**
 * The head of inverse map holds the entry with minimal pa-va pair.
 * (pairs are compared element-wise)
 * `INV_MAP_HEAD.prev' is reserved to maintain list of free entries.
 **/
void inv_map_init(void)
{
    uint16_t i;
    inv_map_head = 0;
    inv_map_tail = INVALID_IDX;
    for (i = 1; i < INVERSE_MAP_ENTRY_NUM; ++i) {
        inv_map[i - 1].next = i;
        inv_map[i].prev = i - 1;
    }
    INV_MAP_HEAD.next = INVALID_IDX;
    INV_MAP_HEAD.prev = 1;
    inv_map[INVERSE_MAP_ENTRY_NUM - 1].next = INVALID_IDX;
}

static uint16_t inv_map_next_avail(void)
{
    uint16_t i = INV_MAP_HEAD.prev;
    if (i == INVALID_IDX) {
        em_error("Running out of entries!\n");
        ecall_exit_enclave(-1);
        __builtin_unreachable();
    }
    inv_map[inv_map[i].next].prev = inv_map_head;
    INV_MAP_HEAD.prev = inv_map[i].next;
    em_debug("Next entry at #%d\n", i);
    em_debug("INV_MAP_HEAD.prev = %d\n", INV_MAP_HEAD.prev);
    return i;
}

inverse_map_t* inv_map_insert(uintptr_t pa, uintptr_t va, uint32_t count)
{
    uint16_t i = inv_map_head, j;

    em_debug("Checkpoint 0, inv_map_tail=%d\n", inv_map_tail);
    // 0. First entry
    if (inv_map_tail == INVALID_IDX) {
        inv_map_tail = inv_map_head;
        inv_map[i].count = count;
        goto assign_va_pa;
    }

    em_debug("Checkpoint 1\n");
    // 1. Check if the new entry should be the head
    if (pairwise_less(pa, va, INV_MAP_HEAD.pa, INV_MAP_HEAD.va)) {
        if (addr_check(pa, INV_MAP_HEAD.pa, count) && addr_check(va, INV_MAP_HEAD.va, count)) {
            // 1-a. Join to the head
            INV_MAP_HEAD.count += count;
        } else {
            // 1-b. New head entry
            i = inv_map_next_avail();
            inv_map[i].count = count;
            inv_map[i].prev = INV_MAP_HEAD.prev;
            inv_map[i].next = inv_map_head;
            INV_MAP_HEAD.prev = i;
            inv_map_head = i;
        }
        goto assign_va_pa;
    }

    em_debug("Checkpoint 2: i=%d\n", i);
    // 2. Check if joinable to the end
    for (; i != INVALID_IDX && inv_map[i].pa < pa; i = inv_map[i].next) {
        if (addr_check(inv_map[i].pa, pa, count) && addr_check(inv_map[i].va, va, count)) {
            inv_map[i].count += count;
            goto ret;
        }
    }

    em_debug("Checkpoint 3\n");
    // 3. Find a new position
    while (i != INVALID_IDX && inv_map[i].pa == pa && inv_map[i].va < va) {
        i = inv_map[i].next;
    }
    if (i != INVALID_IDX) {
        // 3-a. Mark the position to be inserted
        j = i;
    } else {
        // 3-b. Add new entry to the end
        i = inv_map_next_avail();
        inv_map[i].count = count;
        inv_map[i].prev = inv_map_tail;
        inv_map[i].next = -1;
        inv_map[inv_map_tail].next = i;
        inv_map_tail = i;
        goto assign_va_pa;
    }

    em_debug("Checkpoint 4: j=%d\n", j);
    // 4. Check if joinable to the beginning
    for (; i != INVALID_IDX; i = inv_map[i].next) {
        if (addr_check(pa, inv_map[i].pa, count) && addr_check(va, inv_map[i].va, count)) {
            break;
        }
        if (addr_shift(pa) + count < addr_shift(inv_map[i].pa)) {
            i = INVALID_IDX;
            break;
        }
    }
    if (i != INVALID_IDX) {
        // 4-a. Join entries
        inv_map[i].count += count;
        inv_map[inv_map[i].prev].next = inv_map[i].next;
        if (inv_map[i].next != INVALID_IDX) {
            inv_map[inv_map[i].next].prev = inv_map[i].prev;
        }
    } else {
        // 4-b. New entry
        i = inv_map_next_avail();
        inv_map[i].count = count;
    }
    inv_map[i].next = j;
    inv_map[i].prev = inv_map[j].prev;
    inv_map[inv_map[j].prev].next = i;
    inv_map[j].prev = i;

assign_va_pa:
    em_debug("Checkpoint 5: inv_map[%d].next = %d\n", i, inv_map[i].next);
    inv_map[i].pa = pa;
    inv_map[i].va = va;
ret:
    return &inv_map[i];
}