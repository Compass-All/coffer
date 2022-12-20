#pragma once

#include <types.h>
#include <util/register.h>
#include <util/gnu_attribute.h>

typedef struct {
	u64 time;
	u64 last;
} eval_timer_t;

#define DEF_TIMER(name) eval_timer_t timer_##name
#define EXT_TIMER(name) extern eval_timer_t timer_##name

#define START_TIMER(name) timer_##name.last = read_csr(cycle);
#define STOP_TIMER(name) timer_##name.time += (read_csr(cycle) - timer_##name.last);

#define GET_TIMER_VALUE(name) timer_##name.time

__unused void set_s_timer();

EXT_TIMER(interrupt);
EXT_TIMER(syscall);
EXT_TIMER(module);