#ifndef EBI_MEMORY_H
#define EBI_MEMORY_H

#include <stdint.h>
#ifndef ROUNDUP
#define ROUNDUP(a, b) ((((a)-1) / (b) + 1) * (b))
#endif

#define PARTITION_SHIFT 21 // 2 MB for each parition
#define PARTITION_SIZE (1UL << PARTITION_SHIFT)

#define PARTITION_UP(addr) (ROUNDUP(addr, PARTITION_SIZE))
#define PARTITION_DOWN(addr) ((addr) & (~((PARTITION_SIZE)-1)))

#define PAGE_FREE 0x0
#define PAGE_USED 0x1

#define PTE_ATTR_MASK 0xFFFFFC00
#define EPTE_SHIFT 10

/* Based on 64 bits Sv39 Page */
#define SATP_MODE_SV39 8
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

#define PAGE_UP(addr) (ROUNDUP(addr, EPAGE_SIZE))
#define PAGE_DOWN(addr) ((addr) & (~((EPAGE_SIZE)-1)))

#define NUM_OF_PAGE(size) \
    (((PAGE_UP(size)) >> EPAGE_SHIFT) & ((1 << (64 - EPAGE_SHIFT)) - 1))

#define ERT_VA_START 0xC0000000
#define EMEM_SIZE PARTITION_SIZE
#define ERT_MEM_SIZE 0x200000
#define ERT_STACK_SIZE 0x8000
#define EUSR_MEM_SIZE (EMEM_SIZE - ERT_MEM_SIZE)

// virtual page number masks
#define MASK_OFFSET 0xfff
#define MASK_L0 0x1ff000
#define MASK_L1 0x3fe00000
#define MASK_L2 0x7fc0000000

#define INVERSE_MAP_ENTRY_NUM 1024
#define PAGE_DIR_POOL 256

#ifndef __ASSEMBLER__

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

typedef struct inverse_map {
    uintptr_t pa;
    uintptr_t va;
    uint32_t count;
} inverse_map_t;

#endif // __ASSEMBLER__

#endif // EBI_MEMORY_H