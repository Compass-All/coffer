#pragma once

#include <types.h>

/**
 * Converts an invalid pointer (see PTRISERR) to an error number
 * @param ptr Pointer
 * @return error number
 */
#ifndef PTR2ERR
#define PTR2ERR(ptr)					\
	((int) ((__sptr)(ptr)))
#endif

/**
 * Creates an invalid pointer containing an error number (see PTRISERR)
 * @param err Error number
 * @return Pointer
 */
#ifndef ERR2PTR
#define ERR2PTR(err)					\
	((void *) ((__sptr)(err)))
#endif