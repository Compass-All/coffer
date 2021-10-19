#ifndef EBI_UTIL_H
#define EBI_UTIL_H

#define PMP_REGION_MAX 4

#ifndef __ASSEMBLER__
#include "stdint.h"
#include "stddef.h"

#define EBI_DEBUG
#define EBI_EVAL

#ifdef EBI_DEBUG
#define sbi_debug(fmt, ...) sbi_printf("[%s] " fmt, __func__, ##__VA_ARGS__)
#else
#define sbi_debug(fmt, ...)
#endif
#define sbi_error(fmt, ...)                                         \
	sbi_printf("\033[1;41m[%s ERROR] " fmt "\033[0m", __func__, \
		   ##__VA_ARGS__)

typedef struct {
	uintptr_t reg_pa_start;
	uintptr_t reg_va_start;
	uintptr_t reg_size;
	int holder;
} peri_addr_t;

typedef struct {
	uintptr_t pmp_start;
	uintptr_t pmp_size;
	uintptr_t used;
} pmp_region;

#endif // __ASSEMBLER__

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define CLAMP(a, lo, hi) MIN(MAX(a, lo), hi)

#define ROUND_UP(a, b) ((((a)-1) / (b) + 1) * (b))
#define ROUND_DOWN(a, b) ((a) / (b) * (b))

#define EXTRACT_FIELD(val, which) (((val) & (which)) / ((which) & ~((which)-1)))
#define INSERT_FIELD(val, which, fieldval) \
	(((val) & ~(which)) | ((fieldval) * ((which) & ~((which)-1))))

#define STR(x) XSTR(x)
#define XSTR(x) #x

// what's that?
#if __riscv_xlen == 64
#define SLL32 sllw
#define STORE sd
#define LOAD ld
#define LWU lwu
#define LOG_REGBYTES 3
#else
#define SLL32 sll
#define STORE sw
#define LOAD lw
#define LWU lw
#define LOG_REGBYTES 2
#endif

#define CONTEXT_ADDR(reg, base) (##reg##_INDEX * REGBYTES)(base)
#define CONTEXT_ADDR_SP(reg) CONTEXT_ADDR(reg, sp)

// TODO(wtf? to be optimized)
#define RA_INDEX 1
#define SP_INDEX 2
#define GP_INDEX 3
#define TP_INDEX 4
#define T0_INDEX 5
#define T1_INDEX 6
#define T2_INDEX 7
#define S0_INDEX 8
#define S1_INDEX 9
#define A0_INDEX 10
#define A1_INDEX 11
#define A2_INDEX 12
#define A3_INDEX 13
#define A4_INDEX 14
#define A5_INDEX 15
#define A6_INDEX 16
#define A7_INDEX 17
#define S2_INDEX 18
#define S3_INDEX 19
#define S4_INDEX 20
#define S5_INDEX 21
#define S6_INDEX 22
#define S7_INDEX 23
#define S8_INDEX 24
#define S9_INDEX 25
#define S10_INDEX 26
#define S11_INDEX 27
#define T3_INDEX 28
#define T4_INDEX 29
#define T5_INDEX 30
#define T6_INDEX 31
#define MAX_INDEX 32

#define REGBYTES (1 << LOG_REGBYTES)
#define INTEGER_CONTEXT_SIZE (32 * REGBYTES)
#define GPR_CONTEXT_SIZE (MAX_INDEX * (REGBYTES))

#define EBI_OK 0
#define EBI_ERROR -1

#define MASK(n) ((1 << (n)) - 1)

#endif // EBI_UTIL_H