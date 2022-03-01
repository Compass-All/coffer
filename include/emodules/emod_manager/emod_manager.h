#pragma once

#include <emodules/emodule_info.h>

#define EMOD_MANAGER_API_QUERY_MODULE	0

typedef struct {
	emodule_info_t info;

	emodule_info_t (*query_module)(u32 mod_id);
} emod_manager_t;

void test();