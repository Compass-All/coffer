#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>

typedef struct {
	void *(*malloc)(usize size);
	void *(*calloc)(usize number, usize size);
	void (*free)(void *ptr);
} emod_alloc_api_t;

typedef struct {
	emod_desc_t 		emod_alloc_desc;
	emod_alloc_api_t 	emod_alloc_api;
} emod_alloc_t;