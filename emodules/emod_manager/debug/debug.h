#pragma once

#include "../printf/printf.h"
#include <types.h>
#include <util/console.h>
#include <enclave/enclave_ops.h>

#ifdef __DEBUG__
#define EMODULES_DEBUG
#endif

#ifdef EMODULES_DEBUG
#define debug(fmt, ...) \
	printf("\033[37m[E%luT%lu][%s] " fmt "\033[0m", __ecall_ebi_get_eid(), __ecall_ebi_get_tid(), __func__, ##__VA_ARGS__)
#endif
#ifdef EMODULES_DEBUG
#define info(fmt, ...) \
	printf(KMAG "[E%luT%lu][%s] " fmt RESET, __ecall_ebi_get_eid(), __ecall_ebi_get_tid(),__func__, ##__VA_ARGS__)
#endif

#ifndef debug
#define debug(fmt, ...)
#endif
#ifndef info
#define info(fmt, ...)
#endif

#define show(v) debug(#v "\t=\t0x%lx\n", (v));
#define LOG(v) 	printf("[%s]" #v "\t=\t0x%lx\n\r", __func__, (v))

#define error(fmt, ...) \
	printf("\033[1;31m[ERROR] [%s] " fmt "\033[0m",\
	__func__, ##__VA_ARGS__)

void hexdump(vaddr_t addr, usize len);
void assert(u8 *ptr1, u8 *ptr2, usize len);