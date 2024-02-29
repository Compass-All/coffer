#pragma once

#include <types.h>
#include <sbi/riscv_asm.h>
#include <sbi/ebi/enclave.h>
#include <util/gnu_attribute.h>

typedef struct {
	u64 time;
	u64 last;
} eval_timer_t;

typedef u64 s_timer_t;

// ---------------

#define DEF_TIMER(name) eval_timer_t timer_##name[NUM_ENCLAVE + 1]
#define EXT_TIMER(name) extern eval_timer_t timer_##name[NUM_ENCLAVE + 1]

#define INIT_TIMER(name, eid) timer_##name[eid].last = 0UL; timer_##name[eid].time = 0UL;
#define START_TIMER(name, eid) timer_##name[eid].last = csr_read(CSR_MCYCLE);
#define STOP_TIMER(name, eid) timer_##name[eid].time += (csr_read(CSR_MCYCLE) - timer_##name[eid].last);

#define GET_TIMER_VALUE(name, eid) timer_##name[eid].time

#define DEF_S_TIMER(name) s_timer_t s_timer_##name[NUM_ENCLAVE + 1]
#define EXT_S_TIMER(name) extern s_timer_t s_timer_##name[NUM_ENCLAVE + 1]
#define SET_S_TIMER(name, eid, val) s_timer_##name[eid] = (val)
#define GET_S_TIMER(name, eid) (s_timer_##name[eid])

#define DEF_COUNTER(name) u64 count_##name[NUM_ENCLAVE + 1]
#define EXT_COUNTER(name) extern u64 count_##name[NUM_ENCLAVE + 1]

#define INIT_COUNTER(name, eid) count_##name[eid] = 0;
#define ADD_COUNTER(name, eid) count_##name[eid]++;

#define GET_COUNTER_VALUE(name, eid) count_##name[eid]

// ---------------

// #define DEF_TIMER(name)
// #define EXT_TIMER(name)

// #define START_TIMER(name, eid)
// #define STOP_TIMER(name, eid)

// #define GET_TIMER_VALUE(name, eid)

// ---------------

void init_timer(u64 eid);
u64 get_timer(u64 eid, u64 timer);
__unused void dump_timer(u64 eid);

EXT_TIMER(mem_alloc);
EXT_TIMER(lpmp);
EXT_TIMER(mem_migration);
EXT_TIMER(message);
EXT_TIMER(creation);
EXT_TIMER(execution);
EXT_TIMER(clean_up);
EXT_TIMER(total);

EXT_S_TIMER(interrupt);
EXT_S_TIMER(syscall);
EXT_S_TIMER(emodule);

EXT_COUNTER(lpmp);
EXT_COUNTER(mem_migration);