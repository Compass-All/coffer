#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>
#include <stddef.h>

typedef struct {
	void *(*kmalloc)(size_t size);
	void *(*malloc)(size_t size);
	void *(*calloc)(size_t number, size_t size);
	void *(*memalign)(size_t size, size_t align);
	void (*free)(void *ptr);
} emod_alloc_api_t;

typedef struct {
	emod_desc_t 		emod_alloc_desc;
	emod_alloc_api_t 	emod_alloc_api;
} emod_alloc_t;