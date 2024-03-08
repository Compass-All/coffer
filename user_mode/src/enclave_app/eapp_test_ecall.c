#include <enclave_util.h>
#include <stdio.h>
#include <stdlib.h>
#include <types.h>

#define SYSCALL(__syscall_num)     \
    ({                                                          \
        register u64 a0 asm("a0");                              \
        register u64 a7 asm("a7") = (u64)(__syscall_num); 		\
        asm volatile("ecall"                                    \
                    : "+r"(a0)                                  \
                    : "r"(a7)                                   \
                    : "memory");                                \
    })

int main(int argc, char *argv[])
{
    if (argc != 2) {
        printf("Usage: %s [ecall_num]\n", argv[0]);
        return 1;
    }

    u64 ecall_num = (u64)atoi(argv[1]);
    SYSCALL(ecall_num);
}
