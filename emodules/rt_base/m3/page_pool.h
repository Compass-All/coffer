#pragma once

#include <stddef.h>
#include <stdint.h>

// See address definitions in config.mk

#define IDX_USR 0
#define IDX_RT 1
#define NUM_POOL 2

#define ERT_MEM_SHIFT 21
#define ERT_MEM_MASK (~(-1UL << ERT_MEM_SHIFT))

#define INVERSE_MAP_ENTRY_NUM 1024
#define EUSR_STACK_START (EUSR_STACK_TOP - EUSR_STACK_SIZE)

typedef struct page_list {
    uintptr_t head;
    uintptr_t tail;
    unsigned int count;
} page_list_t;

typedef struct page_list_entry {
    uintptr_t next;
} page_list_entry_t;

#define PAGE_NEXT(page) ((page_list_entry_t*)(page))->next
#define LIST_EMPTY(list) ((list)->count == 0 || (list)->head == 0)
#define LIST_INIT(list)    \
    do {                   \
        (list)->count = 0; \
        (list)->head = 0;  \
        (list)->tail = 0;  \
    } while (0)

void page_pool_init(uintptr_t pool_addr, uintptr_t pa_start, size_t pool_size, int idx);
size_t page_pool_get(int idx, uintptr_t* pva, size_t n_pages);