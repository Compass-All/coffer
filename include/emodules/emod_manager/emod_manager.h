#pragma once

#include <emodules/emodule_info.h>

#define MESSAGE_LOAD_MODULE	0x717DB77A

typedef struct {
	emodule_info_t info;

	emodule_info_t (*query_module)(u32 mod_id);
} emod_manager_t;

void load_emodule();