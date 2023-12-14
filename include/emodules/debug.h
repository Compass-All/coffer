#pragma once

#include <enclave/enclave_ops.h>
#include <util/console.h>

#define LOG_LEVEL_DEBUG
#define LOG_LEVEL_INFO

#define LOG_PREFIX_STR  "[Core %lu][E%luT%lu][%s] "
#define LOG_PREFIX_ARG  __ecall_ebi_get_hartid(), __ecall_ebi_get_eid(), __ecall_ebi_get_tid(), __func__

#ifndef IN_EMOD_MANAGER_DEBUG

#include <emodules/emod_debug/emod_debug.h>
#define print   printd

#else   // IN_EMOD_MANAGER_DEBUG

#ifdef __DEBUG__
#define EMODULES_DEBUG
#endif

#ifdef EMODULES_DEBUG
#define print   printf
#else
#define print(fmt, ...)
#endif

#endif  // IN_EMOD_MANAGER_DEBUG

#ifdef LOG_LEVEL_DEBUG
#define debug(fmt, ...) print(KWHT LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)
#else
#define debug(fmt, ...)
#endif

#ifdef LOG_LEVEL_INFO
#define info(fmt, ...) print(KMAG LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)
#else
#define info(fmt, ...)
#endif

#define DEBUG(fmt, ...) \
	printf(KWHT LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)

#define show(v) 	debug(#v "\t=\t0x%lx\n", (v))
#define LOG(v) 		DEBUG(#v "\t=\t0x%lx\n", (v))

#define warn(fmt, ...) \
	printf(BYEL LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)

#define error(fmt, ...) \
	printf(KRED LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)
