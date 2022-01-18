#include "timer.h"
#include <ebi_ecall.h>
#include <stdint.h>
#include <types.h>

#define SBI_ECALL(__extid, __funid, __arg0, __arg1, __arg2)     \
    ({                                                          \
        register uintptr_t a0 asm("a0") = (uintptr_t)(__arg0);  \
        register uintptr_t a1 asm("a1") = (uintptr_t)(__arg1);  \
        register uintptr_t a2 asm("a2") = (uintptr_t)(__arg2);  \
        register uintptr_t a6 asm("a6") = (uintptr_t)(__funid); \
        register uintptr_t a7 asm("a7") = (uintptr_t)(__extid); \
        asm volatile("ecall"                                    \
                     : "+r"(a0)                                 \
                     : "r"(a1), "r"(a2), "r"(a6), "r"(a7)       \
                     : "memory");                               \
    })

static u64 read_time_csr()
{
	u64 time = 0;
    SBI_ECALL(SBI_EXT_EBI, SBI_EXT_EBI_GET_TIME, 0, 0, 0);
    asm volatile("mv %0, a0": "=r"(time));
    return time;
}

u64 get_timer(u64 base)
{
    return read_time_csr() - base;
}

void udelay(u64 usec)
{
	ulong endtime = get_timer(0) + usec;
	signed long diff;

	do {
		ulong now = get_timer(0);
		diff = endtime - now;
	} while (diff >= 0);
}