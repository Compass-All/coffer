#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

#include <types.h>
#include <memory/page_table.h>

#ifndef ROUNDUP
#define ROUNDUP(a, b) ((((a)-1) / (b) + 1) * (b))
#endif

#ifndef __RISCV_ASM_H__
#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#endif

#define PAGE_UP(addr) (ROUNDUP(addr, PAGE_SIZE))
#define PAGE_DOWN(addr) ((addr) & (~((PAGE_SIZE)-1)))

#define PARTITION_SHIFT 21 // 2 MB for each partition
#define PARTITION_SIZE (1UL << PARTITION_SHIFT)

#define PARTITION_UP(addr) (ROUNDUP(addr, PARTITION_SIZE))
#define PARTITION_DOWN(addr) ((addr) & (~((PARTITION_SIZE)-1)))

#define NUM_REGIONS 3

#endif