#ifndef EBI_MEMNGR_H
#define EBI_MEMNGR_H

#include <sbi/sbi_trap.h>
#include <types.h>
#include <sbi/ebi/partition_pool.h>

int ebi_mem_alloc_handler(struct sbi_trap_regs *regs);
// int ebi_addr_record_handler(struct sbi_trap_regs *regs);

#endif // !EBI_MEMNGR_H