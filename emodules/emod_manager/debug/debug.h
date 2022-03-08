#pragma once

#include "../printf/printf.h"
#include <types.h>

#ifdef EMODULES_DEBUG
#define debug(fmt, ...) \
	printf("\033[37m" fmt "\033[0m", ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif

#define error(fmt, ...) \
	printf("\033[1;31m[ERROR] " fmt "\033[0m", ##__VA_ARGS__)

void hexdump(vaddr_t addr, usize len);