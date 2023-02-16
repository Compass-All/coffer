#pragma once

#include <types.h>

u64 get_eid();
void syscall_handler(u64 *regs, u64 sepc, u64 scause, u64 stval);