#pragma once

#include <sbi/sbi_console.h>
#include <sbi/sbi_hart.h>
#include <types.h>
#include <util/gnu_attribute.h>
#include <util/console.h>
#include <sbi/ebi/enclave.h>
#include <sbi/riscv_asm.h>

#ifdef __SBI_DEBUG__
#define EBI_DEBUG
#endif

#define EBI_EVAL

#define LOG_LEVEL_DEBUG
#define LOG_LEVEL_INFO

#define LOG_PREFIX "[Core %u][E%luT%lu][%s] "

#ifdef EBI_DEBUG
#ifdef LOG_LEVEL_DEBUG
#define sbi_debug(fmt, ...) sbi_printf(KWHT LOG_PREFIX fmt RESET, current_hartid(), get_current_eid(), get_current_tid(),  __func__, ##__VA_ARGS__)
#endif
#ifdef LOG_LEVEL_INFO
#define sbi_info(fmt, ...) sbi_printf(KBLU LOG_PREFIX fmt RESET, current_hartid(), get_current_eid(), get_current_tid(), __func__, ##__VA_ARGS__)
#endif
#endif

#ifndef sbi_debug
#define sbi_debug(fmt, ...)
#endif
#ifndef sbi_info
#define sbi_info(fmt, ...)
#endif


#define sbi_warn(fmt, ...) sbi_printf(BYEL LOG_PREFIX fmt RESET, current_hartid(), get_current_eid(), get_current_tid(), __func__, ##__VA_ARGS__)

#define sbi_INFO(fmt, ...) sbi_printf(KBLU LOG_PREFIX fmt RESET, current_hartid(), get_current_eid(),  get_current_tid(), __func__, ##__VA_ARGS__)
#define sbi_DEBUG(fmt, ...) sbi_printf(KWHT LOG_PREFIX fmt RESET, current_hartid(), get_current_eid(),  get_current_tid(), __func__, ##__VA_ARGS__)

#define sbi_error(fmt, ...)                                         \
    sbi_printf(KRED "[Core %u][Enclave %lu][%s] ERROR: " fmt RESET, \
        current_hartid(), get_current_eid(), __func__,              \
        ##__VA_ARGS__)
#define sbi_fatal(fmt, ...)                                         \
    do {                                                            \
        sbi_error(fmt, ##__VA_ARGS__);                              \
        sbi_hart_hang();                                            \
    } while (0)

#define EBI_OK 0
#define EBI_ERROR -1

// #define disable_intr() csr_read_clear(CSR_MSTATUS, MSTATUS_MIE)
#define disable_intr() csr_swap(CSR_MIE, 0)

// #define restore_intr(v) csr_write(CSR_MSTATUS, v)
#define restore_intr(v) csr_swap(CSR_MIE, v)
#define show(v) sbi_debug(#v "\t=\t0x%lx\n", (u64)(v));
#define LOG(v)  sbi_printf("[Core %u][%s] " #v "\t=\t0x%lx\n", current_hartid(), __func__, (u64)(v));

#define ASSERT(x, msg) sbi_assert(x, msg)
__unused void sbi_assert(u64 val, char *assert_msg);

__unused void hexdump(paddr_t addr, usize len);
