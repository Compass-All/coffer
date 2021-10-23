#include "md2.h"

static const uint8_t S[256] = {
    0x29, 0x2E, 0x43, 0xC9, 0xA2, 0xD8, 0x7C, 0x01, 0x3D, 0x36, 0x54, 0xA1, 0xEC, 0xF0, 0x06, 0x13, //
    0x62, 0xA7, 0x05, 0xF3, 0xC0, 0xC7, 0x73, 0x8C, 0x98, 0x93, 0x2B, 0xD9, 0xBC, 0x4C, 0x82, 0xCA, //
    0x1E, 0x9B, 0x57, 0x3C, 0xFD, 0xD4, 0xE0, 0x16, 0x67, 0x42, 0x6F, 0x18, 0x8A, 0x17, 0xE5, 0x12, //
    0xBE, 0x4E, 0xC4, 0xD6, 0xDA, 0x9E, 0xDE, 0x49, 0xA0, 0xFB, 0xF5, 0x8E, 0xBB, 0x2F, 0xEE, 0x7A, //
    0xA9, 0x68, 0x79, 0x91, 0x15, 0xB2, 0x07, 0x3F, 0x94, 0xC2, 0x10, 0x89, 0x0B, 0x22, 0x5F, 0x21, //
    0x80, 0x7F, 0x5D, 0x9A, 0x5A, 0x90, 0x32, 0x27, 0x35, 0x3E, 0xCC, 0xE7, 0xBF, 0xF7, 0x97, 0x03, //
    0xFF, 0x19, 0x30, 0xB3, 0x48, 0xA5, 0xB5, 0xD1, 0xD7, 0x5E, 0x92, 0x2A, 0xAC, 0x56, 0xAA, 0xC6, //
    0x4F, 0xB8, 0x38, 0xD2, 0x96, 0xA4, 0x7D, 0xB6, 0x76, 0xFC, 0x6B, 0xE2, 0x9C, 0x74, 0x04, 0xF1, //
    0x45, 0x9D, 0x70, 0x59, 0x64, 0x71, 0x87, 0x20, 0x86, 0x5B, 0xCF, 0x65, 0xE6, 0x2D, 0xA8, 0x02, //
    0x1B, 0x60, 0x25, 0xAD, 0xAE, 0xB0, 0xB9, 0xF6, 0x1C, 0x46, 0x61, 0x69, 0x34, 0x40, 0x7E, 0x0F, //
    0x55, 0x47, 0xA3, 0x23, 0xDD, 0x51, 0xAF, 0x3A, 0xC3, 0x5C, 0xF9, 0xCE, 0xBA, 0xC5, 0xEA, 0x26, //
    0x2C, 0x53, 0x0D, 0x6E, 0x85, 0x28, 0x84, 0x09, 0xD3, 0xDF, 0xCD, 0xF4, 0x41, 0x81, 0x4D, 0x52, //
    0x6A, 0xDC, 0x37, 0xC8, 0x6C, 0xC1, 0xAB, 0xFA, 0x24, 0xE1, 0x7B, 0x08, 0x0C, 0xBD, 0xB1, 0x4A, //
    0x78, 0x88, 0x95, 0x8B, 0xE3, 0x63, 0xE8, 0x6D, 0xE9, 0xCB, 0xD5, 0xFE, 0x3B, 0x00, 0x1D, 0x39, //
    0xF2, 0xEF, 0xB7, 0x0E, 0x66, 0x58, 0xD0, 0xE4, 0xA6, 0x77, 0x72, 0xF8, 0xEB, 0x75, 0x4B, 0x0A, //
    0x31, 0x44, 0x50, 0xB4, 0x8F, 0xED, 0x1F, 0x1A, 0xDB, 0x99, 0x8D, 0x33, 0x9F, 0x11, 0x83, 0x14, //
};

#define CLEAR_64BITS(p) *((uint64_t*)(p)) = 0x0
#define CLEAR_BLOCK(p)        \
    CLEAR_64BITS((void*)(p)); \
    CLEAR_64BITS((void*)(p) + 8)

#define COPY_AS_TYPE(dst, src, type) *((type*)(dst)) = *((type*)(src))

static void md2_memcpy(void* dst, const void* src, size_t len)
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

static void md2_memset(void* dst, uint8_t byte, size_t len)
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

int md2_init(md2ctx_t* ctx)
{
    if (ctx == NULL) {
        return MD2_NULLPTR;
    }

    CLEAR_BLOCK(ctx->hash_buf);
    CLEAR_BLOCK(ctx->blk1);
    CLEAR_BLOCK(ctx->blk2);
    ctx->last.used_cnt = 0;
    CLEAR_BLOCK(ctx->last.buf);
    CLEAR_BLOCK(ctx->checksum);

    return MD2_OK;
}

static int md2_update_checksum(md2ctx_t* ctx, const uint8_t* block)
{
    uint8_t c, L;
    int i;
    if (ctx == NULL || block == NULL) {
        return MD2_NULLPTR;
    }

    L = ctx->checksum[MD2_BLOCK_SIZE - 1];
    for (i = 0; i < MD2_BLOCK_SIZE; ++i) {
        c = block[i];
        ctx->checksum[i] ^= S[c ^ L];
        L = ctx->checksum[i];
    }

    return MD2_OK;
}

static int md2_prepare_blk(md2ctx_t* ctx, const uint8_t* block)
{
    int i;
    if (ctx == NULL || block == NULL) {
        return MD2_NULLPTR;
    }

    for (i = 0; i < MD2_BLOCK_SIZE; ++i) {
        ctx->blk1[i] = block[i];
        ctx->blk2[i] = ctx->blk1[i] ^ ctx->hash_buf[i];
    }
    return MD2_OK;
}

static int md2_process_block(md2ctx_t* ctx, const void* block)
{
    int ret_state = MD2_OK;
    int i, j;
    uint8_t tmp;
    if (ctx == NULL || block == NULL) {
        return MD2_NULLPTR;
    }

    ret_state = md2_update_checksum(ctx, (uint8_t*)block);
    if (ret_state != MD2_OK) {
        return ret_state;
    }
    ret_state = md2_prepare_blk(ctx, (uint8_t*)block);
    if (ret_state != MD2_OK) {
        return ret_state;
    }

    tmp = 0;
    for (i = 0; i < 18; ++i) {
        for (j = 0; j < 16; ++j) {
            ctx->hash_buf[j] ^= S[tmp];
            tmp = ctx->hash_buf[j];
        }
        for (j = 0; j < 16; ++j) {
            ctx->blk1[j] ^= S[tmp];
            tmp = ctx->blk1[j];
        }
        for (j = 0; j < 16; ++j) {
            ctx->blk2[j] ^= S[tmp];
            tmp = ctx->blk2[j];
        }
        tmp += i;
    }

    return MD2_OK;
}

int md2_update(md2ctx_t* ctx, const void* data, size_t size)
{
    size_t copied_len = 0;
    int ret_state = MD2_OK;

    if (ctx == NULL || data == NULL) {
        return MD2_NULLPTR;
    }

    if (ctx->last.used_cnt != 0) {
        if (ctx->last.used_cnt + size < MD2_BLOCK_SIZE) {
            md2_memcpy(&ctx->last.buf[ctx->last.used_cnt], data,
                size);
            ctx->last.used_cnt += size;
            return MD2_OK;
        } else {
            copied_len = MD2_BLOCK_SIZE - ctx->last.used_cnt;
            md2_memcpy(&ctx->last.buf[ctx->last.used_cnt], data,
                copied_len);
            ret_state = md2_process_block(ctx, ctx->last.buf);
            if (ret_state != MD2_OK) {
                return ret_state;
            }
            data = (uint8_t*)data + copied_len;
            size -= copied_len;
            ctx->last.used_cnt = 0;
            CLEAR_BLOCK(ctx->last.buf);
        }
    }

    while (size >= MD2_BLOCK_SIZE) {
        ret_state = md2_process_block(ctx, data);
        if (ret_state != MD2_OK) {
            return ret_state;
        }
        data = (uint8_t*)data + MD2_BLOCK_SIZE;
        size -= MD2_BLOCK_SIZE;
    }
    md2_memcpy(ctx->last.buf, data, size);
    ctx->last.used_cnt = size;

    return MD2_OK;
}

int md2_final(void* md, md2ctx_t* ctx)
{
    uint32_t padding_len;
    int ret_state;
    if (md == NULL || ctx == NULL) {
        return MD2_NULLPTR;
    }

    padding_len = MD2_BLOCK_SIZE - ctx->last.used_cnt;
    md2_memset(&ctx->last.buf[ctx->last.used_cnt], (uint8_t)padding_len,
        padding_len);
    ret_state = md2_process_block(ctx, ctx->last.buf);
    if (ret_state != MD2_OK) {
        return ret_state;
    }
    ctx->last.used_cnt = 0;

    md2_memcpy(ctx->last.buf, ctx->checksum, MD2_BLOCK_SIZE);
    ret_state = md2_process_block(ctx, ctx->last.buf);
    if (ret_state != MD2_OK) {
        return ret_state;
    }
    md2_memcpy(md, ctx->hash_buf, MD2_BLOCK_SIZE);
    return MD2_OK;
}

void* md2(const void* in, size_t insize, void* md)
{
    md2ctx_t ctx;
    if (in == NULL || md == NULL) {
        return NULL;
    }

    md2_init(&ctx);
    md2_update(&ctx, in, insize);
    md2_final(md, &ctx);

    return md;
}