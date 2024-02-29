#include <types.h>

#ifdef __INTELLISENSE__
// Suppress intellisense warning for unknown registers (1118)
#pragma diag_suppress 1118
#endif

int main()
{
    register u64 extid asm("a7") = 0xDEAD;
    asm volatile ("ecall" :: "r"(extid) : "memory");
    return 0;
}