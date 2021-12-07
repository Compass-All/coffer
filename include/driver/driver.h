#pragma once

#include <stdint.h>

// Module ID Table
#define MOD_UART	0
#define MOD_DUMMY	1

typedef struct {
    uintptr_t pt_root_addr;
    uintptr_t inverse_map_addr;
    uintptr_t lmap_offset_addr;
    uintptr_t module_table_addr;
} addr_record_t;

typedef struct {
	// should imply how many addr regions are used
	uint8_t _dummy;
} peripheral_t;

typedef struct {
	// void (*initializer)(extra_module_t *self); the initializer should always be put 
	// at start_addr, which is responsible for initializing the following values

	uint32_t id;
	uint8_t is_sharable;

	// start_addr is physical address before initializing, written by SM
	// should be modified to virtual address when setup by rt_base
	uintptr_t start_addr;
	uintptr_t size;

	// each module should contain a peripheral list
	peripheral_t *peri_list;

	uintptr_t (*handler)(uint8_t _dummy);
	uintptr_t (*interrupt_handler)(uint8_t _dummy);
} extra_module_t;

typedef int (*initializer)(volatile extra_module_t *emod);