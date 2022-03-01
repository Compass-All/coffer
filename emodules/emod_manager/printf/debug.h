#pragma once

#include "printf.h"

#ifdef EMODULES_DEBUG
// #define debug(fmt, ...) printf("\033[37m[%s] " fmt "\033[0m", __func__, ##__VA_ARGS__)
#define debug(fmt, ...) printf("\033[37m" fmt "\033[0m", ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif
// #define error(fmt, ...) printf("\033[1;31m[%s ERROR] " fmt "\033[0m", __func__, ##__VA_ARGS__)
#define error(fmt, ...) printf("\033[1;31m[ERROR] " fmt "\033[0m", ##__VA_ARGS__)
