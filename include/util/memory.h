#ifndef EBI_MEMORY_H
#define EBI_MEMORY_H

#include "enclave.h"

#define PAGE_FREE 0x0
#define PAGE_USED 0x1

#define PTE_ATTR_MASK 0xFFFFFC00
#define EPTE_SHIFT 10

/* Based on 64 bits Sv39 Page */
#define SATP_MODE_SHIFT 60
#define EPAGE_SHIFT 12
#define EPAGE_SIZE (1 << EPAGE_SHIFT)
#define EMEGA_PAGE_SHIFT 21
#define EMEGA_PAGE_SIZE (1 << EMEGA_PAGE_SHIFT)

#define EPT_LEVEL 3
#define EPT_LEVEL_BITS 9

#define EPPN_SHIFT(level) (EPAGE_SHIFT + (level)*EPT_LEVEL_BITS)
#define EPPN(addr, level) \
	(((addr) >> EPPN_SHIFT(level)) & ((1 << EPT_LEVEL_BITS) - 1))

#define PAGE_UP(addr) (ROUND_UP(addr, EPAGE_SIZE))
#define PAGE_DOWN(addr) ((addr) & (~((EPAGE_SIZE)-1)))

#define NUM_OF_PAGE(size) \
	(((PAGE_UP(size)) >> EPAGE_SHIFT) & ((1 << (64 - EPAGE_SHIFT)) - 1))

#define SECTION_SHIFT 23 // should be less than or equal to 26
#define SECTION_SIZE (1UL << SECTION_SHIFT)

#define SECTION_UP(addr) (ROUND_UP(addr, SECTION_SIZE))
#define SECTION_DOWN(addr) ((addr) & (~((SECTION_SIZE)-1)))

#define EDRV_VA_START 0xC0000000
#define EMEM_SIZE SECTION_SIZE
#define EDRV_MEM_SIZE 0x200000
#define EDRV_STACK_SIZE 0x8000
#define EUSR_MEM_SIZE (EMEM_SIZE - EDRV_MEM_SIZE)

// virtual page number masks
#define MASK_OFFSET 0xfff
#define MASK_L0 0x1ff000
#define MASK_L1 0x3fe00000
#define MASK_L2 0x7fc0000000

// make sure these addresses are section aligned
#define MEMORY_POOL_START 0x140000000
// #define MEMORY_POOL_START 0x180000000
// #define MEMORY_POOL_END (0x280000000 - SECTION_SIZE * 32)
#define MEMORY_POOL_SIZE 0x30000000
#define MEMORY_POOL_END (MEMORY_POOL_SIZE + MEMORY_POOL_START)
#define MEMORY_POOL_SECTION_NUM \
	((MEMORY_POOL_END - MEMORY_POOL_START) >> SECTION_SHIFT)

#define INVERSE_MAP_ENTRY_NUM 1024
#define PAGE_DIR_POOL 256

#ifndef __ASSEMBLER__
// #include "sbi/riscv_locks.h"
typedef struct pte {
	uint32_t pte_v : 1;
	uint32_t pte_r : 1;
	uint32_t pte_w : 1;
	uint32_t pte_x : 1;
	uint32_t pte_u : 1;
	uint32_t pte_g : 1;
	uint32_t pte_a : 1;
	uint32_t pte_d : 1;
	uint32_t rsw : 2;
	uintptr_t ppn : 44;
	uintptr_t __unused_value : 10;
} pte_t;

// TODO(this struct can be optimized)
typedef struct section {
	uintptr_t sfn; // section frame number
	uintptr_t va;  // linearly mapped addr of the section
	int owner;     // enclave id of the owner. -1 if unused.
} section_t;

extern section_t memory_pool[MEMORY_POOL_SECTION_NUM];
// extern spinlock_t memory_pool_lock;

typedef struct inverse_map {
	uintptr_t pa;
	uintptr_t va;
	uint32_t count;
} inverse_map_t;

void dump_section_ownership();
void flush_tlb(void);
void init_memory_pool(void);
uintptr_t alloc_section_for_enclave(enclave_context_t *ectx, uintptr_t va);
void free_section_for_enclave(int eid);
int section_migration(uintptr_t src_sfn, uintptr_t dst_sfn);
void memcpy_from_user(uintptr_t maddr, uintptr_t uaddr, uintptr_t size,
		      uintptr_t mepc);
			  
#ifdef EBI_DEBUG
void debug_memdump(uintptr_t addr, size_t size);
#endif

#endif // __ASSEMBLER__

#endif // EBI_MEMORY_H