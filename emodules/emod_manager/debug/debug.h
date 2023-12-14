#pragma once

#include "../printf/printf.h"
#include <types.h>

#define IN_EMOD_MANAGER_DEBUG

#include <emodules/debug.h>

void hexdump(vaddr_t addr, usize len);
void assert(u8 *ptr1, u8 *ptr2, usize len);