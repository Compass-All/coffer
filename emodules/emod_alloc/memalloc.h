#pragma once

#include <stddef.h>

void *kmalloc(size_t size);
void *malloc(size_t size);
void *calloc(size_t number, size_t size);
void free(void *ptr);