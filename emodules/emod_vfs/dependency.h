#pragma once

#include <types.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emod_alloc/emod_alloc.h>
#include <emodules/emod_debug/emod_debug.h>

extern emod_manager_t 	emod_manager;
extern emod_alloc_t		emod_alloc;
extern emod_debug_t 	emod_debug;

#define malloc 	emod_alloc.emod_alloc_api.malloc
#define calloc 	emod_alloc.emod_alloc_api.calloc
#define free	emod_alloc.emod_alloc_api.free

#define debug		emod_debug.emod_debug_api.printd
#define show(v) 	debug(#v "\t=\t0x%lx\n", (v))

#define panic	emod_manager.emod_manager_api.panic