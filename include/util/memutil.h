#ifndef EBI_MEMUTIL_H
#define EBI_MEMUTIL_H

/* This file contains auxillary function definitions for `memory.c'. 
   *** INTERNAL USE ONLY! ***
 */

#include "memory.h"

extern int compacted;

typedef struct {
	uintptr_t sfn;
	size_t length;
} region_t;

#define for_each_section_in_pool(pool, section, i)                   \
	for (i = 0, section = &pool[i]; i < MEMORY_POOL_SECTION_NUM; \
	     i++, section   = &pool[i])

#define for_each_section_in_pool_rev(pool, section, i)                    \
	for (i = MEMORY_POOL_SECTION_NUM - 1, section = &pool[i]; i >= 0; \
	     i--, section			      = &pool[i])

#define sfn_to_section(sfn) \
	&memory_pool[((sfn) - (MEMORY_POOL_START >> SECTION_SHIFT))]

uint8_t load_uint8_t(const uint8_t *addr, uintptr_t mepc);
uint32_t load_uint32_t(const uint32_t *addr, uintptr_t mepc);
uint64_t load_uint64_t(const uint64_t *addr, uintptr_t mepc);

section_t *find_available_section();
uintptr_t alloc_section_for_host_os();
int get_avail_pmp_count(enclave_context_t *ectx);
region_t find_largest_avail();
region_t find_smallest_region(int eid);
region_t find_avail_region_larger_than(int length);
void page_compaction(void);
void update_tree_pte(uintptr_t root, uintptr_t pa_diff);
void update_leaf_pte(uintptr_t root, uintptr_t va, uintptr_t pa);
void set_section_zero(uintptr_t sfn);
void update_section_info(uintptr_t sfn, int owner, uintptr_t va);
void free_section(uintptr_t sfn);

#endif // EBI_MEMUTIL_H