#pragma once

#include <types.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emod_alloc/emod_alloc.h>
#include <emodules/emod_debug/emod_debug.h>
#include <util/console.h>

extern emod_manager_t 	emod_manager;
extern emod_alloc_t		emod_alloc;
extern emod_debug_t 	emod_debug;

#define kmalloc emod_alloc.emod_alloc_api.kmalloc
#define calloc 	emod_alloc.emod_alloc_api.calloc
#define memalign	emod_alloc.emod_alloc_api.memalign
#define free	emod_alloc.emod_alloc_api.free

#define printd		emod_debug.emod_debug_api.printd
#define printf		emod_debug.emod_debug_api.printf
#define hexdump		emod_debug.emod_debug_api.hexdump
#define start_timer	emod_debug.emod_debug_api.start_timer
#define call_timer	emod_debug.emod_debug_api.call_timer
#define end_timer	emod_debug.emod_debug_api.end_timer
#define debug(fmt, ...) \
	printd(KWHT "[%s] " fmt RESET, __func__, ##__VA_ARGS__)
#define info(fmt, ...) \
	printd(KMAG "[%s] " fmt RESET, __func__, ##__VA_ARGS__)
#define DEBUG(fmt, ...) \
	printf(KWHT "[%s] " fmt RESET, __func__, ##__VA_ARGS__)
#define show(v) 	debug(#v "\t=\t0x%lx\n", (v))
#define LOG(v) 		printf(#v "\t=\t0x%lx\n", (v))

#define panic		emod_manager.emod_manager_api.panic
#define map_page	emod_manager.emod_manager_api.map_page
#define unmap_page	emod_manager.emod_manager_api.unmap_page

#define get_pa	emod_manager.emod_manager_api.get_pa