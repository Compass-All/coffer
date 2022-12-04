#ifndef HEAP_H
#define HEAP_H

#include <stdint.h>
#include <stddef.h>
#include <util/gnu_attribute.h>
#include <types.h>
#include <memory/memory.h>

#define HEAP_INIT_PARTITION_NUM 1
#define HEAP_INIT_SIZE (HEAP_INIT_PARTITION_NUM * PARTITION_SIZE)
#define HEAP_MAX_SIZE 0xFFFFFFFFFFFFFFFF
#define HEAP_MIN_SIZE HEAP_INIT_SIZE

#define MIN_ALLOC_SZ 4

#define MIN_WILDERNESS 0x10000
#define MAX_WILDERNESS 0x0FFFFFFF

// #define MIN_WILDERNESS 0xD000000
// #define MAX_WILDERNESS 0x200000000

#define BIN_COUNT 0x2c
#define BIN_MAX_IDX (BIN_COUNT - 1)

typedef struct node_t {
    uint hole;
    size_t size;
    struct node_t* next;
    struct node_t* prev;
} node_t;

typedef struct { 
    node_t *header;
} footer_t;

typedef struct {
    node_t* head;
} bin_t;

typedef struct {
    vaddr_t start;
    vaddr_t end;
    bin_t *bins[BIN_COUNT];
} heap_t;

void init_heap();

void *heap_alloc(size_t size);
void *heap_calloc(size_t number, size_t size);
void *heap_memalign(size_t size, size_t align);
void heap_free(void *p);
uint expand(size_t sz);
void contract(size_t sz);

uint get_bin_index(size_t sz);
size_t get_index_by_bin(bin_t *bin);
void create_foot(node_t *head);
footer_t *get_foot(node_t *head);

__unused void display_chunk(node_t *head);

node_t *get_wilderness();

#endif
