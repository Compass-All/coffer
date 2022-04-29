#pragma once

#include <types.h>

void init_memory_pool();
void *kmalloc(usize size);
void *malloc(usize size);
void *calloc(usize number, usize size);
void free(void *ptr);