#ifndef EBI_IOMNGR_H
#define EBI_IOMNGR_H

#include <sbi/sbi_trap.h>
#include <types.h>

void clear_device(u64 eid);

int init_device(void);
void device_update_permission(u64 eid);
void io_schedule(struct sbi_trap_regs* regs);
int match_device_for_enclave(u64 eid, paddr_t pa);

#endif