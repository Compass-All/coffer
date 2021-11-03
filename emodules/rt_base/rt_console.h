#pragma once

#include "rt_ecall.h"

#define EMODULES_DEBUG

void rt_printf(const char* fmt, ...);

#ifdef EMODULES_DEBUG
#define em_debug(fmt, ...) rt_printf("\033[37m[%s] " fmt "\033[0m", __func__, ##__VA_ARGS__)
#else
#define em_debug(fmt, ...)
#endif
#define em_error(fmt, ...) rt_printf("\033[1;31m[%s ERROR] " fmt "\033[0m", __func__, ##__VA_ARGS__)
