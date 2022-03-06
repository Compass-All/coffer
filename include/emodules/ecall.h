#pragma once
#include <types.h>

#define SBI_ECALL(__extid, __funid, __arg0, __arg1, __arg2)     \
    ({                                                          \
        register u64 a0 asm("a0") = (u64)(__arg0);  			\
        register u64 a1 asm("a1") = (u64)(__arg1);  			\
        register u64 a2 asm("a2") = (u64)(__arg2);  			\
        register u64 a6 asm("a6") = (u64)(__funid); 			\
        register u64 a7 asm("a7") = (u64)(__extid); 			\
        asm volatile("ecall"                                    \
                    : "+r"(a0)                                  \
                    : "r"(a1), "r"(a2), "r"(a6), "r"(a7)        \
                    : "memory");                                \
    })

static u64 inline __ecall(
    u64 __extid,
    u64 __funid,
    u64 __arg0,
    u64 __arg1,
    u64 __arg2
)
{
    u64 ret;
    SBI_ECALL(__extid, __funid, __arg0, __arg1, __arg2);
    asm volatile("mv %0, a0": "=r"(ret));
    return ret;
}