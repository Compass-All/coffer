#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>

typedef struct {
	void (*test)(int arg1);
} emod_net_api_t;

typedef struct {
	emod_desc_t 		emod_net_desc;
	emod_net_api_t 		emod_net_api;
} emod_net_t;