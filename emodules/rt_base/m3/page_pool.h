#pragma once

#include <stddef.h>
#include <stdint.h>

// See address definitions in config.mk

#define IDX_USR 0
#define IDX_RT 1
#define NUM_POOL 2

#define ERT_MEM_SHIFT 21
#define ERT_MEM_MASK (~(-1UL << ERT_MEM_SHIFT))

#define EUSR_MEM_SIZE (EMEM_SIZE - ERT_MEM_SIZE)
#define INVERSE_MAP_ENTRY_NUM 1024
#define EUSR_STACK_START (EUSR_STACK_TOP - EUSR_STACK_SIZE)

#define NEXT_PAGE(page) *((uintptr_t*)page)
#define LIST_EMPTY(list) ((list)->count == 0 || (list)->head == 0)
#define LIST_INIT(list)    \
    {                      \
        (list)->count = 0; \
        (list)->head = 0;  \
        (list)->tail = 0;  \
    }

typedef struct page_list {
    uintptr_t head;
    uintptr_t tail;
    unsigned int count;
} page_list_t;

void page_pool_init(uintptr_t pool_addr, size_t pool_size, int idx);
uintptr_t page_pool_get_pa(int idx, size_t n_pages);