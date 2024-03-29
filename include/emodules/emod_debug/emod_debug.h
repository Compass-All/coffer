#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>

typedef struct {
	int (*printd)(const char* format, ...);
	int (*printf)(const char* format, ...);
	int (*snprintf)(char* buffer, size_t count, const char* format, ...);

	void (*hexdump)(vaddr_t addr, usize len);
	void (*cmp_check)(u8 *ptr1, u8 *ptr2, usize len);

	void (*start_timer)();
	void (*call_timer)();
	void (*end_timer)();
} emod_debug_api_t;

typedef struct {
	emod_desc_t 		emod_debug_desc;
	emod_debug_api_t	emod_debug_api;
} emod_debug_t;