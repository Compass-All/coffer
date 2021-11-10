#pragma once

#include <stdint.h>

#define PDMA_NUM			4

#define PDMA_BASE_ADDR		0x3000000UL
#define PDMA_SIZE			0xFFFFFUL
#define PDMA_OFFSET 		0x80000UL

#define PDMA_CONTROL_OFFSET			0x000
#define PDMA_NEXT_CONFIG_OFFSET		0x004
#define PDMA_NEXT_BYTES_OFFSET		0x008
#define PDMA_NEXT_DST_OFFSET		0x010
#define PDMA_NEXT_SRC_OFFSET		0x018
#define PDMA_EXEC_CONFIG_OFFSET		0x104
#define PDMA_EXEC_BYTES_OFFSET		0x108
#define PDMA_EXEC_DST_OFFSET		0x110
#define PDMA_EXEC_SRC_OFFSET		0x118

// n represents PDMA channel ID, ranging from 0 to 3
#define PDMA_CONTROL(n)		(PDMA_BASE_ADDR + PDMA_OFFSET + (0x1000 * n) \
							 + PDMA_CONTROL_OFFSET)
#define PDMA_NEXT_CONFIG(n)	(PDMA_BASE_ADDR + PDMA_OFFSET + (0x1000 * n) \
							 + PDMA_NEXT_CONFIG_OFFSET)
#define PDMA_NEXT_BYTES(n)	(PDMA_BASE_ADDR + PDMA_OFFSET + (0x1000 * n) \
							 + PDMA_NEXT_BYTES_OFFSET)
#define PDMA_NEXT_DST(n)	(PDMA_BASE_ADDR + PDMA_OFFSET + (0x1000 * n) \
							 + PDMA_NEXT_DST_OFFSET)
#define PDMA_NEXT_SRC(n)	(PDMA_BASE_ADDR + PDMA_OFFSET + (0x1000 * n) \
							 + PDMA_NEXT_SRC_OFFSET)
#define PDMA_EXEC_CONFIG(n)	(PDMA_BASE_ADDR + PDMA_OFFSET + (0x1000 * n) \
							 + PDMA_EXEC_CONFIG_OFFSET)
#define PDMA_EXEC_BYTES(n)	(PDMA_BASE_ADDR + PDMA_OFFSET + (0x1000 * n) \
							 + PDMA_EXEC_BYTES_OFFSET)
#define PDMA_EXEC_DST(n)	(PDMA_BASE_ADDR + PDMA_OFFSET + (0x1000 * n) \
							 + PDMA_EXEC_DST_OFFSET)
#define PDMA_EXEC_SRC(n)	(PDMA_BASE_ADDR + PDMA_OFFSET + (0x1000 * n) \
							 + PDMA_EXEC_SRC_OFFSET)

typedef struct {
	uintptr_t src_addr;
	uintptr_t dst_addr;

	uintptr_t size;
} dma_data;

typedef struct {
	uint32_t claim			: 1;	// 0
	uint32_t run			: 1;	// 1
	uint32_t __reserved1	: 12;	// [13:2]
	uint32_t done_ie		: 1;	// 14
	uint32_t error_ie		: 1;	// 15
	uint32_t __reserved2	: 14;	// [29:16]
	uint32_t done 			: 1;	// 30
	uint32_t error 		: 1;	// 31
} dma_control;

typedef struct {
	uint32_t __reserved1	: 2;	// [1:0]
	uint32_t repeat 		: 1;	// 2
	uint32_t order 		: 1;	// 3
	uint32_t __reserved2	: 20;	// [23:4]
	uint32_t wsize			: 4;	// [27:24]
	uint32_t rsize			: 4;	// [31:27]
} dma_config;

void dma_debug();