#pragma once

#include <types.h>
#include <stdint.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>

/* second argument to futex syscall */
#define FUTEX_WAIT		0
#define FUTEX_WAKE		1
#define FUTEX_FD		2
#define FUTEX_REQUEUE   3

#define FUTEX_PRIVATE_FLAG	128
#define FUTEX_CLOCK_REALTIME	256
#define FUTEX_CMD_MASK		~(FUTEX_PRIVATE_FLAG | FUTEX_CLOCK_REALTIME)

typedef struct {
	int (*sys_futex_handler)(uint32_t *uaddr, int futex_op, uint32_t val,
		uintptr_t _2, uint32_t *_3, uint32_t _4);
} emod_futex_api_t;

typedef struct {
	emod_desc_t 		emod_futex_desc;
	emod_futex_api_t 	emod_futex_api;
} emod_futex_t;