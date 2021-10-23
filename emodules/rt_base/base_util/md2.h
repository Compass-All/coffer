#pragma once

#include <stddef.h>
#include <stdint.h>

#define MD2_BLOCK_SIZE 16

typedef struct md2_context {
    uint8_t hash_buf[MD2_BLOCK_SIZE];
    uint8_t blk1[MD2_BLOCK_SIZE], blk2[MD2_BLOCK_SIZE];

    struct {
        uint32_t used_cnt;
        uint8_t buf[MD2_BLOCK_SIZE];
    } last;

    uint8_t checksum[MD2_BLOCK_SIZE];
} md2ctx_t;

enum md2_state { MD2_OK,
    MD2_ERR,
    MD2_NULLPTR };

int md2_init(md2ctx_t* ctx);
int md2_update(md2ctx_t* ctx, const void* data, size_t size);
int md2_final(void* md, md2ctx_t* ctx);
void* md2(const void* in, size_t insize, void* md);