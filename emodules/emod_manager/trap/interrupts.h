#pragma once

#include <types.h>

void interrupt_handler(u64 *regs, u64 sepc, u64 scause, u64 stval);