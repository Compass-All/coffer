#pragma once

#include <types.h>
#include <util/gnu_attribute.h>
#include <sbi/sbi_list.h>

typedef struct {
	usize count;
	struct sbi_dlist reg_list;	// list head
} enclave_region_t;

typedef struct {
	paddr_t pa;
	usize	size;

    int     is_inst;    // whether the region contains instruction
	usize	count;

	struct sbi_dlist entry;
} region_t;

typedef struct {
	paddr_t pa;
	usize 	size;
} region_info_t;

enclave_region_t *get_enclave_regions(u64 eid);
region_info_t get_smallest_region(u64 eid);
void lock_region();
void unlock_region();

void enclave_add_region(u64 eid, paddr_t pa, usize size, int is_inst);
void __enclave_add_region(u64 eid, paddr_t pa, usize size, int is_inst);
void __enclave_remove_region(u64 eid, paddr_t pa);
int enclave_hit_region(u64 eid, paddr_t pa);
void enclave_free_regions(u64 eid);
void region_init();
void region_reset();

__unused void __dump_region_list(u64 eid);
__unused void dump_region_list(u64 eid);
__unused void dump_region();