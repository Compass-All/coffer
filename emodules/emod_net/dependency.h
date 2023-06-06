#pragma once

#include <types.h>
#include <util/console.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emod_alloc/emod_alloc.h>
#include <emodules/emod_debug/emod_debug.h>

extern emod_manager_t   emod_manager;
extern emod_alloc_t		emod_alloc;
extern emod_debug_t 	emod_debug;

#define map_page	emod_manager.emod_manager_api.map_page

#define printf		emod_debug.emod_debug_api.printf
#define snprintf	emod_debug.emod_debug_api.snprintf
#define printd		emod_debug.emod_debug_api.printd
#define debug(fmt, ...) \
	printd(KWHT "[%s] " fmt RESET, __func__, ##__VA_ARGS__)

