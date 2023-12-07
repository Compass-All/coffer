#pragma once

#include <types.h>

int futex_init(void);
int sys_futex_handler(u32 *uaddr, int futex_op, int val, u64 _2, u64 _3);