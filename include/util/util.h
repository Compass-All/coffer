#ifndef EBI_UTIL_H
#define EBI_UTIL_H

#define PMP_REGION_MAX 4

#ifndef __ASSEMBLER__
#include "stdint.h"
#include "stddef.h"

//TODO (still, this address seems to be optimized)
typedef struct {
	uintptr_t reg_pa_start;
	uintptr_t reg_va_start;
	uint32_t reg_size;
	int holder;
} peri_addr_t;

// typedef struct {
// 	uintptr_t pmp_start;
// 	uintptr_t pmp_size;
// 	uintptr_t used;
// } memseg_t;

#endif // __ASSEMBLER__
#endif // EBI_UTIL_H