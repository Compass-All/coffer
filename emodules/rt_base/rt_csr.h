#pragma once

// CSR operations

#define read_csr(reg)                 \
    ({                                \
        unsigned long __tmp;          \
        asm volatile("csrr %0, " #reg \
                     : "=r"(__tmp));  \
        __tmp;                        \
    })

#define write_csr(reg, val) ({ asm volatile("csrw " #reg ", %0" ::"rK"(val)); })

#define clear_csr(reg, bit)                   \
    ({                                        \
        unsigned long __tmp;                  \
        asm volatile("csrrc %0, " #reg ", %1" \
                     : "=r"(__tmp)            \
                     : "rK"(bit));            \
        __tmp;                                \
    })

// CSR field definitions

#define SSTATUS_UIE 0x00000001UL
#define SSTATUS_UPIE 0x00000010UL
#define SSTATUS_SPP 0x00000100UL
#define SSTATUS_SUM 0x00040000UL

#define SIE_SSIE 0x00000002UL
#define SIE_SEIE 0x00000200UL

#define SIP_SSIP 0x00000002UL
#define SIP_STIP 0x00000020UL