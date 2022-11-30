#pragma once

#include <types.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emod_debug/emod_debug.h>

#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

extern emod_manager_t 	emod_manager;
extern emod_debug_t 	emod_debug;

#define map_page	emod_manager.emod_manager_api.map_page
#define get_pa 		emod_manager.emod_manager_api.get_pa

#define printd		emod_debug.emod_debug_api.printd
#define printf		emod_debug.emod_debug_api.printf
#define hexdump		emod_debug.emod_debug_api.hexdump
#define debug(fmt, ...) \
	printd(KWHT "[%s] " fmt RESET, __func__, ##__VA_ARGS__)
#define info(fmt, ...) \
	printd(KMAG "[%s] " fmt RESET, __func__, ##__VA_ARGS__)
#define show(v) 	debug(#v "\t=\t0x%lx\n", (v))
#define LOG(v)		printf("[%s]" #v "\t=\t0x%lx\n", __func__, (v))

#define panic	emod_manager.emod_manager_api.panic