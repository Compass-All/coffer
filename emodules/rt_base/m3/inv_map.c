#include "inv_map.h"
#include "../rt_console.h"
#include "../rt_ecall.h"

inverse_map_t inv_map[INVERSE_MAP_ENTRY_NUM];
static uint16_t inv_map_tail;

#define PN(addr) ((addr) >> EPAGE_SHIFT)

#define MAX(a, b) ((a) < (b) ? (b) : (a))

#define INVALID_IDX ((uint16_t)-1)

/**
 * The head of inverse map holds the entry with minimal pa-va pair.
 * (pairs are compared element-wise)
 * `INV_MAP_HEAD.prev' is reserved to maintain list of free entries.
 **/
void inv_map_init(void)
{
    uint16_t i;
    inv_map_tail = INVALID_IDX;
    for (i = 1; i < INVERSE_MAP_ENTRY_NUM; ++i) {
        inv_map[i - 1].next = i;
        inv_map[i].prev = i - 1;
    }
    inv_map[0].next = INVALID_IDX;
    inv_map[0].prev = 1;
    inv_map[INVERSE_MAP_ENTRY_NUM - 1].next = INVALID_IDX;
}

static uint16_t inv_map_next_avail(void)
{
    uint16_t i = inv_map[0].prev;
    if (i == INVALID_IDX) {
        em_error("Running out of entries!\n");
        ecall_exit_enclave(-1);
        __builtin_unreachable();
    }
    inv_map[inv_map[i].next].prev = 0;
    inv_map[0].prev = inv_map[i].next;
    return i;
}

inverse_map_t* inv_map_insert(uintptr_t pa, uintptr_t va, uint32_t count)
{
    uint16_t i = 0;

    // First entry
    if (inv_map_tail == INVALID_IDX) {
        inv_map_tail = 0;
        inv_map[i].pa = pa;
        inv_map[i].va = va;
        inv_map[i].count = count;
        return &inv_map[i];
    }

    // Try joining
    for (; i != INVALID_IDX; i = inv_map[i].next) {
        if (PN(va) - PN(pa) == PN(inv_map[i].va) - PN(inv_map[i].pa)) {
            if (pa < inv_map[i].pa) {
                if (PN(pa) + count < PN(inv_map[i].pa)) {
                    continue;
                }
                inv_map[i].pa = pa;
                inv_map[i].va = va;
                inv_map[i].count = MAX(count, inv_map[i].count - PN(pa) + PN(inv_map[i].pa));
                return &inv_map[i];
            } else {
                if (PN(inv_map[i].pa) + inv_map[i].count < PN(pa)) {
                    continue;
                }
                inv_map[i].count = MAX(inv_map[i].count, count - PN(inv_map[i].pa) + PN(pa));
                return &inv_map[i];
            }
        }
    }

    // New entry
    i = inv_map_next_avail();
    inv_map[i].pa = pa;
    inv_map[i].va = va;
    inv_map[i].count = count;
    inv_map[i].prev = inv_map_tail;
    inv_map[i].next = INVALID_IDX;
    inv_map[inv_map_tail].next = i;
    inv_map_tail = i;
    return &inv_map[i];
}