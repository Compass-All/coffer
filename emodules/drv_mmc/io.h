#pragma once

#include <types.h>

static inline void __raw_writeb(u8 val, volatile void *addr)
{
	asm volatile("sb %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writew(u16 val, volatile void *addr)
{
	asm volatile("sh %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writel(u32 val, volatile void *addr)
{
	asm volatile("sw %0, 0(%1)" : : "r"(val), "r"(addr));
}

#if __riscv_xlen != 32
static inline void __raw_writeq(u64 val, volatile void *addr)
{
	asm volatile("sd %0, 0(%1)" : : "r"(val), "r"(addr));
}
#endif

static inline u8 __raw_readb(const volatile void *addr)
{
	u8 val;

	asm volatile("lb %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline u16 __raw_readw(const volatile void *addr)
{
	u16 val;

	asm volatile("lh %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline u32 __raw_readl(const volatile void *addr)
{
	u32 val;

	asm volatile("lw %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

#if __riscv_xlen != 32
static inline u64 __raw_readq(const volatile void *addr)
{
	u64 val;

	asm volatile("ld %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}
#endif

/* FIXME: These are now the same as asm-generic */

/* clang-format off */

#define __io_rbr()		do {} while (0)
#define __io_rar()		do {} while (0)
#define __io_rbw()		do {} while (0)
#define __io_raw()		do {} while (0)

#define readb_relaxed(c)	({ u8  __v; __io_rbr(); __v = __raw_readb(c); __io_rar(); __v; })
#define readw_relaxed(c)	({ u16 __v; __io_rbr(); __v = __raw_readw(c); __io_rar(); __v; })
#define readl_relaxed(c)	({ u32 __v; __io_rbr(); __v = __raw_readl(c); __io_rar(); __v; })

#define writeb_relaxed(v,c)	({ __io_rbw(); __raw_writeb((v),(c)); __io_raw(); })
#define writew_relaxed(v,c)	({ __io_rbw(); __raw_writew((v),(c)); __io_raw(); })
#define writel_relaxed(v,c)	({ __io_rbw(); __raw_writel((v),(c)); __io_raw(); })

#if __riscv_xlen != 32
#define readq_relaxed(c)	({ u64 __v; __io_rbr(); __v = __raw_readq(c); __io_rar(); __v; })
#define writeq_relaxed(v,c)	({ __io_rbw(); __raw_writeq((v),(c)); __io_raw(); })
#endif

#define __io_br()	do {} while (0)
#define __io_ar()	__asm__ __volatile__ ("fence i,r" : : : "memory");
#define __io_bw()	__asm__ __volatile__ ("fence w,o" : : : "memory");
#define __io_aw()	do {} while (0)

#define readb(c)	({ u8  __v; __io_br(); __v = __raw_readb(c); __io_ar(); __v; })
#define readw(c)	({ u16 __v; __io_br(); __v = __raw_readw(c); __io_ar(); __v; })
#define readl(c)	({ u32 __v; __io_br(); __v = __raw_readl(c); __io_ar(); __v; })

#define writeb(v,c)	({ __io_bw(); __raw_writeb((v),(c)); __io_aw(); })
#define writew(v,c)	({ __io_bw(); __raw_writew((v),(c)); __io_aw(); })
#define writel(v,c)	({ __io_bw(); __raw_writel((v),(c)); __io_aw(); })

// int __ilog2_u32(u32 n)
// {
// 	return fls(n) - 1;
// }

// int __ilog2_u64(u64 n)
// {
// 	return fls64(n) - 1;
// }

#define ilog2(n)				\
(								\
		(n) < 2 ? 0 :			\
		(n) & (1ULL << 63) ? 63 :	\
		(n) & (1ULL << 62) ? 62 :	\
		(n) & (1ULL << 61) ? 61 :	\
		(n) & (1ULL << 60) ? 60 :	\
		(n) & (1ULL << 59) ? 59 :	\
		(n) & (1ULL << 58) ? 58 :	\
		(n) & (1ULL << 57) ? 57 :	\
		(n) & (1ULL << 56) ? 56 :	\
		(n) & (1ULL << 55) ? 55 :	\
		(n) & (1ULL << 54) ? 54 :	\
		(n) & (1ULL << 53) ? 53 :	\
		(n) & (1ULL << 52) ? 52 :	\
		(n) & (1ULL << 51) ? 51 :	\
		(n) & (1ULL << 50) ? 50 :	\
		(n) & (1ULL << 49) ? 49 :	\
		(n) & (1ULL << 48) ? 48 :	\
		(n) & (1ULL << 47) ? 47 :	\
		(n) & (1ULL << 46) ? 46 :	\
		(n) & (1ULL << 45) ? 45 :	\
		(n) & (1ULL << 44) ? 44 :	\
		(n) & (1ULL << 43) ? 43 :	\
		(n) & (1ULL << 42) ? 42 :	\
		(n) & (1ULL << 41) ? 41 :	\
		(n) & (1ULL << 40) ? 40 :	\
		(n) & (1ULL << 39) ? 39 :	\
		(n) & (1ULL << 38) ? 38 :	\
		(n) & (1ULL << 37) ? 37 :	\
		(n) & (1ULL << 36) ? 36 :	\
		(n) & (1ULL << 35) ? 35 :	\
		(n) & (1ULL << 34) ? 34 :	\
		(n) & (1ULL << 33) ? 33 :	\
		(n) & (1ULL << 32) ? 32 :	\
		(n) & (1ULL << 31) ? 31 :	\
		(n) & (1ULL << 30) ? 30 :	\
		(n) & (1ULL << 29) ? 29 :	\
		(n) & (1ULL << 28) ? 28 :	\
		(n) & (1ULL << 27) ? 27 :	\
		(n) & (1ULL << 26) ? 26 :	\
		(n) & (1ULL << 25) ? 25 :	\
		(n) & (1ULL << 24) ? 24 :	\
		(n) & (1ULL << 23) ? 23 :	\
		(n) & (1ULL << 22) ? 22 :	\
		(n) & (1ULL << 21) ? 21 :	\
		(n) & (1ULL << 20) ? 20 :	\
		(n) & (1ULL << 19) ? 19 :	\
		(n) & (1ULL << 18) ? 18 :	\
		(n) & (1ULL << 17) ? 17 :	\
		(n) & (1ULL << 16) ? 16 :	\
		(n) & (1ULL << 15) ? 15 :	\
		(n) & (1ULL << 14) ? 14 :	\
		(n) & (1ULL << 13) ? 13 :	\
		(n) & (1ULL << 12) ? 12 :	\
		(n) & (1ULL << 11) ? 11 :	\
		(n) & (1ULL << 10) ? 10 :	\
		(n) & (1ULL <<  9) ?  9 :	\
		(n) & (1ULL <<  8) ?  8 :	\
		(n) & (1ULL <<  7) ?  7 :	\
		(n) & (1ULL <<  6) ?  6 :	\
		(n) & (1ULL <<  5) ?  5 :	\
		(n) & (1ULL <<  4) ?  4 :	\
		(n) & (1ULL <<  3) ?  3 :	\
		(n) & (1ULL <<  2) ?  2 :	\
		1							\
)

