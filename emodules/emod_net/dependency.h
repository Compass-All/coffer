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
#define get_pa 		emod_manager.emod_manager_api.get_pa
#define panic 		emod_manager.emod_manager_api.panic

#define printf		emod_debug.emod_debug_api.printf
#define snprintf	emod_debug.emod_debug_api.snprintf
#define printd		emod_debug.emod_debug_api.printd
#define debug(fmt, ...) \
	printf(KWHT "[%s] " fmt RESET, __func__, ##__VA_ARGS__)
#define error(fmt, ...) \
	printf(KRED "[ERROR][%s] " fmt RESET, __func__, ##__VA_ARGS__)
#define info(fmt, ...) \
	printd(KMAG "[%s] " fmt RESET, __func__, ##__VA_ARGS__)
#define warn(fmt, ...) \
	printd(KYEL "[%s] " fmt RESET, __func__, ##__VA_ARGS__)

#define show(v) 	debug(#v "\t=\t0x%lx\n", (v))
#define LOG(v)		printf("[%s]" #v "\t=\t0x%lx\n", __func__, (v))

#define ASSERT(x) \
	do { \
		if (!(x)) { \
            error("[%s: %u] Assertion failed: %s\n", __FILE__, __LINE__, #x); \
			panic("Panic"); \
		} \
	} while (0)