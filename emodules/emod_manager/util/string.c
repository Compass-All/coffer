#include "string.h"
#include <types.h>

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