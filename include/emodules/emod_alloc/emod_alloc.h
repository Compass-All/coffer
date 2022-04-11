#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>

typedef struct {
	int (*alloc_func1)(int arg1, int arg2);
	void (*alloc_func2)(int arg1);
} emod_alloc_api_t;

typedef struct {
	emod_desc_t 		emod_alloc_desc;
	emod_alloc_api_t 	emod_alloc_api;
} emod_alloc_t;