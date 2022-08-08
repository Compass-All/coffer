#pragma once

#include <types.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emod_debug/emod_debug.h>

extern emod_manager_t 	emod_manager;
extern emod_debug_t 	emod_debug;

#define map_page	emod_manager.emod_manager_api.map_page
#define get_pa 		emod_manager.emod_manager_api.get_pa

#define printd		emod_debug.emod_debug_api.printd
#define printf		emod_debug.emod_debug_api.printf
#define hexdump		emod_debug.emod_debug_api.hexdump
#define debug(fmt, ...) \
	printd("\033[37m[%s] " fmt "\033[0m", __func__, ##__VA_ARGS__)
#define show(v) 	debug(#v "\t=\t0x%lx\n", (v))
#define LOG(v)		printf("[%s]" #v "\t=\t0x%lx\n", __func__, (v))

#define panic	emod_manager.emod_manager_api.panic