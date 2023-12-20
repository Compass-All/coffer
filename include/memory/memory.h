#ifndef MEMORY_MEMORY_H
#define MEMORY_MEMORY_H

#include <types.h>
#include <memory/page_table.h>
#include <enclave/threads.h>

#ifndef ROUNDUP
#define ROUNDUP(a, b) ((((a)-1) / (b) + 1) * (b))
#endif

#ifndef ROUNDDOWN
#define ROUNDDOWN(a, b) ((a) / (b) * (b))
#endif

#ifndef __RISCV_ASM_H__
#define PAGE_SHIFT 12
#define PAGE_SIZE (1UL << PAGE_SHIFT)
#define __PAGE_SIZE PAGE_SIZE
#define PAGE_MASK	(~(PAGE_SIZE - 1))
#define __PAGE_MASK	PAGE_MASK
#endif

#define PAGE_UP(addr) (ROUNDUP(addr, PAGE_SIZE))
#define PAGE_DOWN(addr) ((addr) & (~((PAGE_SIZE)-1)))

#define PARTITION_SHIFT 21 // 2 MB for each partition
#define PARTITION_SIZE (1UL << PARTITION_SHIFT)

#define PARTITION_UP(addr) (ROUNDUP(addr, PARTITION_SIZE))
#define PARTITION_DOWN(addr) ((addr) & (~((PARTITION_SIZE)-1)))

#define UMODE_STACK_SIZE_TOTAL 0x80000
#define UMODE_STACK_SIZE   (UMODE_STACK_SIZE_TOTAL / MAX_FORK)
#define UMODE_STACK_TOP_VA 0x3fff000000

#endif