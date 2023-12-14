#pragma once

#include <enclave/enclave_ops.h>
#include <util/console.h>

#define LOG_PREFIX_STR  "[Core %lu][E%luT%lu][%s] "
#define LOG_PREFIX_ARG  __ecall_ebi_get_hartid(), __ecall_ebi_get_eid(), __ecall_ebi_get_tid(), __func__

#ifndef IN_EMOD_MANAGER_DEBUG

#include <emodules/emod_debug/emod_debug.h>

#define debug(fmt, ...) \
	printd(KWHT LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)
#define info(fmt, ...) \
	printd(KMAG LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)

#else   // IN_EMOD_MANAGER_DEBUG

#ifdef __DEBUG__
#define EMODULES_DEBUG
#endif

#ifdef EMODULES_DEBUG
#define debug(fmt, ...) \
	printf(KWHT LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)
#endif
#ifdef EMODULES_DEBUG
#define info(fmt, ...) \
	printf(KMAG LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)
#endif

#ifndef debug
#define debug(fmt, ...)
#endif
#ifndef info
#define info(fmt, ...)
#endif

#endif  // IN_EMOD_MANAGER_DEBUG

#define show(v) 	debug(#v "\t=\t0x%lx\n", (v))
#define LOG(v) 		printf(#v "\t=\t0x%lx\n", (v))

#define DEBUG(fmt, ...) \
	printf(KWHT LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)

#define warn(fmt, ...) \
	printf(BYEL LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)

#define error(fmt, ...) \
	printf(KRED LOG_PREFIX_STR fmt RESET, LOG_PREFIX_ARG, ##__VA_ARGS__)
