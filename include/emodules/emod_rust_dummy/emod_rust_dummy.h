#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>

typedef struct {
	int (*dummy_func1)(int arg1, int arg2);
	void (*dummy_func2)(int arg1);
} emod_rust_dummy_api_t;

typedef struct {
	emod_desc_t 		emod_rust_dummy_desc;
	emod_rust_dummy_api_t 	emod_rust_dummy_api;
} emod_rust_dummy_t;
