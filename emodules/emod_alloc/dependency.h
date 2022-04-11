#pragma once

#include <types.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emod_debug/emod_debug.h>

extern emod_manager_t 	emod_manager;
extern emod_debug_t 	emod_debug;

#define map_page	emod_manager.emod_manager_api.map_page
#define panic		emod_manager.emod_manager_api.panic

#define debug		emod_debug.emod_debug_api.printd
#define show(v) 	debug(#v "\t=\t0x%lx\n", (v))