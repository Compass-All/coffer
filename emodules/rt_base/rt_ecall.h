#pragma once

#include <ebi_ecall.h>

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

#define SBI_ECALL_0(__fun) SBI_ECALL(SBI_EXT_EBI, SBI_EXT_EBI_##__fun, 0, 0, 0)
#define SBI_ECALL_1(__fun, __arg0) SBI_ECALL(SBI_EXT_EBI, SBI_EXT_EBI_##__fun, __arg0, 0, 0)
#define SBI_ECALL_2(__fun, __arg0, __arg1) SBI_ECALL(SBI_EXT_EBI, SBI_EXT_EBI_##__fun, __arg0, __arg1, 0)
#define SBI_ECALL_3(__fun, __arg0, __arg1, __arg2) SBI_ECALL(SBI_EXT_EBI, SBI_EXT_EBI_##__fun, __arg0, __arg1, __arg2)

#define exit_enclave(ret_val) SBI_ECALL_2(EXIT, enclave_id, ret_val)
#define peri_inform(pa, va, size) SBI_ECALL_3(PERI_INFORM, pa, va, size)
#define map_register(pt_root_addr, inv_map_addr, va_pa_offset) SBI_ECALL_3(MAP_REGISTER, pt_root_addr, inv_map_addr, va_pa_offset)