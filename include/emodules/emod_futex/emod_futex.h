#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>

/* second argument to futex syscall */
#define FUTEX_WAIT		0
#define FUTEX_WAKE		1
#define FUTEX_FD		2

#define FUTEX_PRIVATE_FLAG	128
#define FUTEX_CLOCK_REALTIME	256
#define FUTEX_CMD_MASK		~(FUTEX_PRIVATE_FLAG | FUTEX_CLOCK_REALTIME)


typedef struct {
	int (*sys_futex_handler)(u32 *uaddr, int futex_op, int val, u64 _2, u64 _3, u64 _4);
} emod_futex_api_t;

typedef struct {
	emod_desc_t 		emod_futex_desc;
	emod_futex_api_t 	emod_futex_api;
} emod_futex_t;