#include <enclave_util.h>
#include <stdio.h>
#include <enclave/enclave_ops.h>
#include <types.h>
#include <stdlib.h>
#include <util/register.h>

void *memcpy1(void *dst, const void *src, size_t len)
{
	size_t p;

	for (p = 0; p < len; ++p)
		*((__u8 *)(((__uptr)dst) + p)) = *((__u8 *)(((__uptr)src) + p));

	return dst;
}

void *memcpy2(void* dst, const void* src, size_t len)
{
#define COPY_AS_TYPE(dst, src, type) *((type*)(dst)) = *((type*)(src))
    size_t offset = 0;
    while (len > 0) {
        if (len >= 8) {
            COPY_AS_TYPE(dst + offset, src + offset, uint64_t);
            offset += 8;
            len -= 8;
        } else if (len >= 4) {
            COPY_AS_TYPE(dst + offset, src + offset, uint32_t);
            offset += 4;
            len -= 4;
        } else if (len >= 2) {
            COPY_AS_TYPE(dst + offset, src + offset, uint16_t);
            offset += 2;
            len -= 2;
        } else {
            COPY_AS_TYPE(dst + offset, src + offset, uint8_t);
            ++offset;
            --len;
        }
    }

	return dst;
}

void *memcpy3(void* dst, const void* src, size_t len)
{
	void *ret = dst, *ptr = (void *)src;

#define COPY_TYPE(type, d, s) do {	\
	*(type *)d = *(type *)s;		\
	d += sizeof(type);				\
	s += sizeof(type);				\
	len -= sizeof(type);			\
} while (0)

	while (len > 0) { 
		if (len >= sizeof(uint64_t))
			COPY_TYPE(uint64_t, dst, ptr);
		else if (len >= sizeof(uint32_t))
			COPY_TYPE(uint32_t, dst, ptr);
		else if (len >= sizeof(uint16_t))
			COPY_TYPE(uint16_t, dst, ptr);
		else
			COPY_TYPE(uint8_t, dst, ptr);
	}

	return ret;
}

void *memset1(void *ptr, int val, size_t len)
{
	__u8 *p = (__u8 *) ptr;

	for (; len > 0; --len)
		*(p++) = (__u8)val;

	return ptr;
}

// byte must be uint8_t
void *memset2(void* dst, int byte, size_t len)
{
    size_t offset = 0;
    uint64_t fill = byte;
    fill |= fill << 8;
    fill |= fill << 16;
    fill |= fill << 32;
    while (len > 0) {
        if (len > 8) {
            *((uint64_t*)(dst + offset)) = fill;
            offset += 8;
            len -= 8;
        } else if (len > 4) {
            *((uint32_t*)(dst + offset)) = fill & 0xFFFFFFFF;
            offset += 4;
            len -= 4;
        } else if (len > 2) {
            *((uint16_t*)(dst + offset)) = fill & 0xFFFF;
            offset += 2;
            len -= 2;
        } else {
            *((uint8_t*)(dst + offset)) = byte;
            ++offset;
            --len;
        }
    }

	return dst;
}

// #define SIZE (0x100000 * 32) 32M
static u64 SIZE;

static u64 bench_memcpy(void *(*test_memcpy)(void *, const void *, size_t))
{
	char *mem1 = (char *)malloc(SIZE);
	char *mem2 = (char *)malloc(SIZE);
	u64 cycle1, cycle2;

	// clean-up cache
	for (int i = 0; i < SIZE; i++)
		mem1[i] = 1;
	for (int i = 0; i < SIZE; i++)
		mem2[i] = 0;

	cycle1 = read_csr(cycle);
	test_memcpy(mem2, mem1, SIZE);
	cycle2 = read_csr(cycle);

	return cycle2 - cycle1;
}

static u64 bench_memset(void *(*test_memset)(void *, int, size_t))
{
	char *mem = (char *)malloc(SIZE);
	u64 cycle1, cycle2;

	// clean-up cache
	for (int i = 0; i < SIZE; i++)
		mem[i] = 1;

	cycle1 = read_csr(cycle);
	test_memset(mem, 0, SIZE);
	cycle2 = read_csr(cycle);

	return cycle2 - cycle1;
}

int main(int argc, char *argv[])
{
	if (argc != 2) {
		printf("Usage: eapp_memcpy [size]\n");
		exit(0);
	}
	SIZE = (int)strtol(argv[1], NULL, 0);
	printf("size = 0x%lx\n", SIZE);

	u64 cycle;

	printf("benchmark memcpy1:\n");
	cycle = bench_memcpy(memcpy1);
	printf("takes %lu cycles\n", cycle);

	printf("benchmark memcpy2:\n");
	cycle = bench_memcpy(memcpy2);
	printf("takes %lu cycles\n", cycle);

	printf("benchmark memcpy3:\n");
	cycle = bench_memcpy(memcpy3);
	printf("takes %lu cycles\n", cycle);

	printf("benchmark memset1:\n");
	cycle = bench_memset(memset1);
	printf("takes %lu cycles\n", cycle);

	printf("benchmark memset2:\n");
	cycle = bench_memset(memset2);
	printf("takes %lu cycles\n", cycle);
}