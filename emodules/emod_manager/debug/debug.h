#pragma once

#include "../printf/printf.h"
#include <types.h>

#ifdef __DEBUG__
#define EMODULES_DEBUG
#endif

#ifdef EMODULES_DEBUG
#define debug(fmt, ...) \
	printf("\033[37m[%s] " fmt "\033[0m", __func__, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif

#define show(v) debug(#v "\t=\t0x%lx\n", (v));

#define error(fmt, ...) \
	printf("\033[1;31m[ERROR] [%s] " fmt "\033[0m",\
	__func__, ##__VA_ARGS__)

void hexdump(vaddr_t addr, usize len);
void assert(u8 *ptr1, u8 *ptr2, usize len);