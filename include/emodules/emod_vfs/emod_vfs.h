#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>

typedef struct {
	int (*vfs_func1)(int arg1, int arg2);
	void (*vfs_func2)(int arg1);
} emod_vfs_api_t;

typedef struct {
	emod_desc_t 		emod_vfs_desc;
	emod_vfs_api_t 	emod_vfs_api;
} emod_vfs_t;