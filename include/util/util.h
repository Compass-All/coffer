#ifndef EBI_UTIL_H
#define EBI_UTIL_H

#ifndef __ASSEMBLER__
#include "stddef.h"
#include "stdint.h"

// TODO (still, this address seems to be optimized)
typedef struct {
    uintptr_t reg_pa_start;
    uintptr_t reg_va_start;
    uint32_t reg_size;
    int holder;
} peri_addr_t;

#endif // __ASSEMBLER__

#define COFFER_EVAL

#if __riscv_xlen == 64
#define STORE sd
#define LOAD ld
#else
#define STORE sw
#define LOAD lw
#endif

#define CTX_INDEX_ra 1
#define CTX_INDEX_sp 2
#define CTX_INDEX_gp 3
#define CTX_INDEX_tp 4
#define CTX_INDEX_t0 5
#define CTX_INDEX_t1 6
#define CTX_INDEX_t2 7
#define CTX_INDEX_s0 8
#define CTX_INDEX_s1 9
#define CTX_INDEX_a0 10
#define CTX_INDEX_a1 11
#define CTX_INDEX_a2 12
#define CTX_INDEX_a3 13
#define CTX_INDEX_a4 14
#define CTX_INDEX_a5 15
#define CTX_INDEX_a6 16
#define CTX_INDEX_a7 17
#define CTX_INDEX_s2 18
#define CTX_INDEX_s3 19
#define CTX_INDEX_s4 20
#define CTX_INDEX_s5 21
#define CTX_INDEX_s6 22
#define CTX_INDEX_s7 23
#define CTX_INDEX_s8 24
#define CTX_INDEX_s9 25
#define CTX_INDEX_s10 26
#define CTX_INDEX_s11 27
#define CTX_INDEX_t3 28
#define CTX_INDEX_t4 29
#define CTX_INDEX_t5 30
#define CTX_INDEX_t6 31
#define CTX_INDEX_MAX 32

#define EAPP_CTX_REG_OFFSET(reg) (__SIZEOF_POINTER__ * (CTX_INDEX_##reg))
#define EAPP_CTX_SIZE EAPP_CTX_REG_OFFSET(MAX)

#endif // EBI_UTIL_H