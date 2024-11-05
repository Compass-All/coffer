#pragma once

#include <types.h>

/* second argument to futex syscall */
#define FUTEX_WAIT		0
#define FUTEX_WAKE		1
#define FUTEX_FD		2

#define FUTEX_PRIVATE_FLAG	128
#define FUTEX_CLOCK_REALTIME	256
#define FUTEX_CMD_MASK		~(FUTEX_PRIVATE_FLAG | FUTEX_CLOCK_REALTIME)

int futex_init(void);
int sys_futex_handler(u32 *uaddr, int futex_op, int val, u64 _2, u64 _3, u64 _4);