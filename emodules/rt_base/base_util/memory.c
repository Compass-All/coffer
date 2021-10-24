#include "memory.h"

#define COPY_AS_TYPE(dst, src, type) *((type*)(dst)) = *((type*)(src))

void memcpy(void* dst, const void* src, size_t len)
{
    size_t offset = 0;
    while (len > 0) {
        if (len > 8) {
            COPY_AS_TYPE(dst + offset, src + offset, uint64_t);
            offset += 8;
            len -= 8;
        } else if (len > 4) {
            COPY_AS_TYPE(dst + offset, src + offset, uint32_t);
            offset += 4;
            len -= 4;
        } else if (len > 2) {
            COPY_AS_TYPE(dst + offset, src + offset, uint16_t);
            offset += 2;
            len -= 2;
        } else {
            COPY_AS_TYPE(dst + offset, src + offset, uint8_t);
            ++offset;
            --len;
        }
    }
}

void memset(void* dst, uint8_t byte, size_t len)
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
}