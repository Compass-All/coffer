#include "arch/sys_arch.h"
#include "timestamp.h"
#include <string.h>

u32 sys_now()
{
	emod_net_timestamp ts;
	emod_net_timestamp_get(&ts);
    return (u32)ts;
}

void free(void *ptr)
{
    emod_alloc.emod_alloc_api.free(ptr);
}

void *malloc(size_t size)
{
    return emod_alloc.emod_alloc_api.malloc(size);
}

void *calloc(size_t count, size_t size)
{
    return emod_alloc.emod_alloc_api.calloc(count, size);
}

void *memalign(size_t alignment, size_t size)
{
	return emod_alloc.emod_alloc_api.memalign(alignment, size);
}

int atoi(const char *strn)
{
	int ret = 0;
	int i = 0;
	while (strn[i] != '\0') {
		ret = ret * 10 + strn[i] - '0';
		i++;
	}
	return ret;
}

void *memset(void* dst, int byte, size_t len)
{
    size_t offset = 0;
    u64 fill = byte;
    fill |= fill << 8;
    fill |= fill << 16;
    fill |= fill << 32;
    while (len > 0) {
        if (len > 8) {
            *((u64*)(dst + offset)) = fill;
            offset += 8;
            len -= 8;
        } else if (len > 4) {
            *((u32*)(dst + offset)) = fill & 0xFFFFFFFF;
            offset += 4;
            len -= 4;
        } else if (len > 2) {
            *((u16*)(dst + offset)) = fill & 0xFFFF;
            offset += 2;
            len -= 2;
        } else {
            *((u8*)(dst + offset)) = byte;
            ++offset;
            --len;
        }
    }

	return dst;
}

void *memcpy(void *dest, const void *src, size_t count)
{
	char *temp1	  = dest;
	const char *temp2 = src;

	while (count > 0) {
		*temp1++ = *temp2++;
		count--;
	}

	return dest;
}

size_t strlen(const char *str)
{
	size_t len = 0;
	while (str[len] != '\0') {
		len++;
	}
	return len;
}

int memcmp(const void *s1, const void *s2, size_t cnt)
{
	const unsigned char *p1 = s1, *p2 = s2;
	while (cnt-- > 0) {
		if (*p1 != *p2) {
			return *p1 - *p2;
		}
		p1++;
		p2++;
	}
	return 0;
}

paddr_t ukplat_virt_to_phys(const void *vaddr)
{
	return get_pa((vaddr_t)vaddr);
}