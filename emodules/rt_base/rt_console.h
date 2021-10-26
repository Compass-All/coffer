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

#define SBI_EXT_0_1_CONSOLE_PUTCHAR 0x1

#define ecall_putchar(ch)                                                           \
    ({                                                                              \
        register uintptr_t a0 asm("a0") = (uintptr_t)(ch);                          \
        register uintptr_t a7 asm("a7") = (uintptr_t)(SBI_EXT_0_1_CONSOLE_PUTCHAR); \
        asm volatile("ecall"                                                        \
                     : "+r"(a0)                                                     \
                     : "r"(a7)                                                      \
                     : "memory");                                                   \
    })
