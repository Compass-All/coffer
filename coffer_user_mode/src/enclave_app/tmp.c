#include <types.h>

int main()
{
    register u64 extid asm("a7") = 0xDEAD;
    asm volatile ("ecall" :: "r"(extid) : "memory");
    return 0;
}