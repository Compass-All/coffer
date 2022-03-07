#pragma once

#include <emodules/emodule_info.h>
#include <types.h>

#define MESSAGE_LOAD_MODULE	0x717DB77A

typedef struct {
	u32		emod_id;
	char 	name[16];

	void 	*emodule_api;
} emodule_t;