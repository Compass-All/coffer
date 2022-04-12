#pragma once

#include <types.h>

#define	UK_BITS_PER_LONG		64
#define howmany(x, y) ((((x) % (y)) == 0) ? ((x) / (y)) : (((x) / (y)) + 1))
#define	UK_BITS_TO_LONGS(n)               howmany((n), UK_BITS_PER_LONG)

#define	UK_BITMAP_FIRST_WORD_MASK(start)  (~0UL << ((start) % UK_BITS_PER_LONG))
#define	UK_BITMAP_LAST_WORD_MASK(n)       (~0UL >> (UK_BITS_PER_LONG - (n)))

#define UK_BIT_WORD(nr)                   ((nr) / UK_BITS_PER_LONG)

static inline __u32
uk_bitcount32(__u32 _x)
{
	_x = (_x & 0x55555555u) + ((_x & 0xaaaaaaaau) >> 1);
	_x = (_x & 0x33333333u) + ((_x & 0xccccccccu) >> 2);
	_x = (_x + (_x >> 4)) & 0x0f0f0f0fu;
	_x = (_x + (_x >> 8));
	_x = (_x + (_x >> 16)) & 0x000000ffu;
	return _x;
}

static inline __u64
uk_bitcount64(__u64 _x)
{
#if __SIZEOF_LONG__ == 8
	_x = (_x & 0x5555555555555555u) + ((_x & 0xaaaaaaaaaaaaaaaau) >> 1);
	_x = (_x & 0x3333333333333333u) + ((_x & 0xccccccccccccccccu) >> 2);
	_x = (_x + (_x >> 4)) & 0x0f0f0f0f0f0f0f0fu;
	_x = (_x + (_x >> 8));
	_x = (_x + (_x >> 16));
	_x = (_x + (_x >> 32)) & 0x000000ffu;
	return _x;
#else
	return (uk_bitcount32(_x >> 32) + uk_bitcount32(_x));
#endif
}
#define	uk_bitcountl(x)	uk_bitcount64((unsigned long)(x))
#define	uk_hweight_long(x)	uk_bitcountl(x)

static inline unsigned int ukarch_ffsl(unsigned long value)
{
    unsigned pos = 0;
    while (!(value & 1))
    {
        value >>= 1;
        ++pos;
    }
    return pos;
}

static inline int uk_test_bit(int nr, const volatile unsigned long *addr)
{
	const volatile __u8 *ptr = (const __u8 *) addr;
	int ret =  ((1 << (nr & 7)) & (ptr[nr >> 3])) != 0;

	return ret;
}

static inline unsigned long
uk_find_next_zero_bit(const unsigned long *addr, unsigned long size,
	unsigned long offset)
{
	long mask;
	int offs;
	int bit;
	int pos;

	if (offset >= size)
		return (size);
	pos = offset / UK_BITS_PER_LONG;
	offs = offset % UK_BITS_PER_LONG;
	bit = UK_BITS_PER_LONG * pos;
	addr += pos;
	if (offs) {
		mask = ~(*addr) & ~UK_BITMAP_LAST_WORD_MASK(offs);
		if (mask)
			return (bit + ukarch_ffsl(mask));
		if (size - bit <= UK_BITS_PER_LONG)
			return (size);
		bit += UK_BITS_PER_LONG;
		addr++;
	}
	for (size -= bit; size >= UK_BITS_PER_LONG;
		size -= UK_BITS_PER_LONG, bit += UK_BITS_PER_LONG, addr++) {
		if (~(*addr) == 0)
			continue;
		return (bit + ukarch_ffsl(~(*addr)));
	}
	if (size) {
		mask = ~(*addr) & UK_BITMAP_LAST_WORD_MASK(size);
		if (mask)
			bit += ukarch_ffsl(mask);
		else
			bit += size;
	}
	return (bit);
}

static inline unsigned long
uk_find_next_bit(const unsigned long *addr, unsigned long size,
	unsigned long offset)
{
	long mask;
	int offs;
	int bit;
	int pos;

	if (offset >= size)
		return (size);
	pos = offset / UK_BITS_PER_LONG;
	offs = offset % UK_BITS_PER_LONG;
	bit = UK_BITS_PER_LONG * pos;
	addr += pos;
	if (offs) {
		mask = (*addr) & ~UK_BITMAP_LAST_WORD_MASK(offs);
		if (mask)
			return (bit + ukarch_ffsl(mask));
		if (size - bit <= UK_BITS_PER_LONG)
			return (size);
		bit += UK_BITS_PER_LONG;
		addr++;
	}
	for (size -= bit; size >= UK_BITS_PER_LONG;
		size -= UK_BITS_PER_LONG, bit += UK_BITS_PER_LONG, addr++) {
		if (*addr == 0)
			continue;
		return (bit + ukarch_ffsl(*addr));
	}
	if (size) {
		mask = (*addr) & UK_BITMAP_LAST_WORD_MASK(size);
		if (mask)
			bit += ukarch_ffsl(mask);
		else
			bit += size;
	}
	return (bit);
}

enum {
	REG_OP_ISFREE,
	REG_OP_ALLOC,
	REG_OP_RELEASE,
};

#define MIN(a, b)                                                      \
	({                                                                 \
		__typeof__(a) __a = (a);                                       \
		__typeof__(b) __b = (b);                                       \
		__a < __b ? __a : __b;                                         \
	})

static inline int
__uk_bitopts_reg_op(unsigned long *bitmap, int pos, int order, int reg_op)
{
	int nbits_reg;
	int index;
	int offset;
	int nlongs_reg;
	int nbitsinlong;
	unsigned long mask;
	int i;
	int ret = 0;

	nbits_reg = 1 << order;
	index = pos / UK_BITS_PER_LONG;
	offset = pos - (index * UK_BITS_PER_LONG);
	nlongs_reg = UK_BITS_TO_LONGS(nbits_reg);
	nbitsinlong = MIN(nbits_reg,  UK_BITS_PER_LONG);

	mask = (1UL << (nbitsinlong - 1));
	mask += mask - 1;
	mask <<= offset;

	switch (reg_op) {
	case REG_OP_ISFREE:
		for (i = 0; i < nlongs_reg; i++) {
			if (bitmap[index + i] & mask)
				goto done;
		}
		ret = 1;
		break;

	case REG_OP_ALLOC:
		for (i = 0; i < nlongs_reg; i++)
			bitmap[index + i] |= mask;
		break;

	case REG_OP_RELEASE:
		for (i = 0; i < nlongs_reg; i++)
			bitmap[index + i] &= ~mask;
		break;
	}
done:
	return ret;
}