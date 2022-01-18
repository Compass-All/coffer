#pragma once

#include <stdint.h>
#include "uart.h"
#include "mmc.h"

// Module ID Table
#define MOD_NONSHARE_DUMMY	0
#define MOD_NONSHARE_UART	1
#define MOD_NONSHARE_MMC	2

#define MOD_SHARE_START		100
#define MOD_SHARE_DUMMY1 	(MOD_SHARE_START + 0)
#define MOD_SHARE_DUMMY2	(MOD_SHARE_START + 1)

#define NUM_NONSHARE_EXTRA_MODULES	16
#define NUM_SHARE_EXTRA_MODULES		16
#define NUM_EXTRA_MODULES			(NUM_NONSHARE_EXTRA_MODULES + NUM_SHARE_EXTRA_MODULES)

#define PERI_REGION_MAX		16

typedef struct {
    uintptr_t pt_root_addr;
    uintptr_t inverse_map_addr;
    uintptr_t lmap_offset_addr;
    uintptr_t module_table_addr;
} addr_record_t;

typedef struct {
	// should imply how many addr regions are used
	uintptr_t start;
	uintptr_t size;
} peripheral_region_t;

typedef struct {
	peripheral_region_t peri_region[PERI_REGION_MAX];
} peripheral_t;

typedef struct {
	uintptr_t cmd;
	uintptr_t arg[3];
} module_arg_t;

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
	// consider using region_t for peripheral list
	peripheral_t peripheral;

	uintptr_t (*handler)(module_arg_t arg);
	uintptr_t (*interrupt_handler)(module_arg_t arg);
} extra_module_t;

typedef int (*initializer)(volatile extra_module_t *emod);