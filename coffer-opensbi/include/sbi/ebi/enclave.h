#pragma once

#include <types.h>
#include <sbi/sbi_trap.h>
#include <sbi/sbi_list.h>
#include <util/gnu_attribute.h>
#include <enclave/threads.h>
#include "sbi/riscv_locks.h"

#define NUM_CORES	4
void init_enclave_desc();
bool ebi_is_called();

// u64 get_eid_by_entry(struct sbi_dlist *entry);
// u64 get_hartid_by_entry(struct sbi_dlist *entry);
// struct sbi_dlist* get_entry(u64 eid);

int ebi_create_handler(struct sbi_trap_regs* regs);
int ebi_enter_handler(struct sbi_trap_regs* regs);
int ebi_suspend_handler(struct sbi_trap_regs* regs);
int ebi_block_thread_handler(struct sbi_trap_regs *regs);
int ebi_unblock_threads_handler(struct sbi_trap_regs *regs);
int ebi_resume_handler(struct sbi_trap_regs* regs);
int ebi_exit_handler(struct sbi_trap_regs* regs);
int ebi_exit_thread_handler(struct sbi_trap_regs* regs);

int ebi_sm_halt(struct sbi_trap_regs* regs);
int ebi_sm_resume(struct sbi_trap_regs* regs);

int sys_clone_handler(struct sbi_trap_regs* regs);

__unused void DUMP_enclave_context(u64 eid, u64 tid);

u64 get_enclave_sepc(u64 eid, u64 tid);
int dump_enclave_status();
u64 get_alive_count();
u64 get_enclave_satp(u64 eid);
// void set_enclave_satp(u64 eid, u64 satp_value);
u64 get_current_mpp();
u64 get_current_eid(void);
u64 get_current_tid(void);
u8 get_enclave_status(u64 eid);
void reset_coffer();
u64 get_eid_count();
void wait_for_init_enclaves(void);
u64 get_blocked_threads(u64 eid);
u64 get_clear_child_tid(u64 eid, u64 tid);
void set_clear_child_tid(u64 eid, u64 tid, u64 tidptr);
