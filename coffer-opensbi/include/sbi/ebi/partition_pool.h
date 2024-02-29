#pragma once

#include <types.h>
#include <memory/memory.h>
#include <sbi/sbi_trap.h>
#include <util/gnu_attribute.h>

#if defined __QEMU__
#define POOL_SIZE   0x1F0000000UL
#elif defined __UNMATCHED__
#define POOL_SIZE   0x100000000UL
#else
#error "Unknown Platform"
#endif
#define NUM_PARTITIONS (POOL_SIZE / PARTITION_SIZE)

#define IDX_TO_PA(idx) ((paddr_t)(get_pool_base() + ((usize)(idx) << PARTITION_SHIFT)))
#define PA_TO_IDX(pa) ((usize)((paddr_t)(pa) - get_pool_base()) >> PARTITION_SHIFT)

usize get_pool_base();
void enclave_memory_clear(u64 eid);
void wait_for_compaction(void);
int try_region_migration(u64 eid);
paddr_t alloc_partitions_for_enclave(u64 eid,
	usize number_of_partitions, usize *suggestion, int is_inst);
// void addr_record(paddr_t emod_manager_start_pa_ptr,
// 	paddr_t umode_pool_pa_aligned_ptr);
int init_partition_pool(void);
void clear_entire_pool();
int is_in_pool(paddr_t pa);

__unused void stop_other_harts();
__unused void resume_other_harts();
__unused void dump_partition_ownership();