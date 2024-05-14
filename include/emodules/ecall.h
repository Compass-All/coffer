#pragma once
#include <types.h>
#include <util/cpu.h>

#define SBI_ECALL(__extid, __funid, __arg0, __arg1, __arg2)     \
    ({                                                          \
        register u64 a0 __asm__("a0") = (u64)(__arg0);  			\
        register u64 a1 __asm__("a1") = (u64)(__arg1);  			\
        register u64 a2 __asm__("a2") = (u64)(__arg2);  			\
        register u64 a6 __asm__("a6") = (u64)(__funid); 			\
        register u64 a7 __asm__("a7") = (u64)(__extid); 			\
        __asm__ __volatile__("ecall"                                    \
                    : "+r"(a0)                                  \
                    : "r"(a1), "r"(a2), "r"(a6), "r"(a7)        \
                    : "memory");                                \
    })

#define SBI_ECALL_5(        \
    __extid, __funid,       \
    __arg0, __arg1, __arg2, \
    __arg3, __arg4          \
)                           \
    ({                                                          \
        register u64 a0 __asm__("a0") = (u64)(__arg0);  			\
        register u64 a1 __asm__("a1") = (u64)(__arg1);  			\
        register u64 a2 __asm__("a2") = (u64)(__arg2);  			\
        register u64 a3 __asm__("a3") = (u64)(__arg3);  			\
        register u64 a4 __asm__("a4") = (u64)(__arg4);  			\
        register u64 a6 __asm__("a6") = (u64)(__funid); 			\
        register u64 a7 __asm__("a7") = (u64)(__extid); 			\
        __asm__ __volatile__("ecall"                                    \
                    : "+r"(a0)                                  \
                    : "r"(a1), "r"(a2), "r"(a3), "r"(a4),       \
                      "r"(a6), "r"(a7)                          \
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
    FENCE();
    SBI_ECALL(__extid, __funid, __arg0, __arg1, __arg2);
    __asm__ __volatile__("mv %0, a0": "=r"(ret) :: "a0", "a1", "memory");
    FENCE();
    return ret;
}


static u64 inline __ecall_5(
    u64 __extid,
    u64 __funid,
    u64 __arg0,
    u64 __arg1,
    u64 __arg2,
    u64 __arg3,
    u64 __arg4
)
{
    u64 ret;
    FENCE();
    SBI_ECALL_5(__extid, __funid, __arg0, __arg1, __arg2, __arg3, __arg4);
    __asm__ __volatile__("mv %0, a0": "=r"(ret) :: "a0", "a1", "memory");
    FENCE();
    return ret;
}