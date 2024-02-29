#ifndef EBI_PMP_H
#define EBI_PMP_H

#include <types.h>
#include <util/gnu_attribute.h>

// void pmp_enable(int idx, paddr_t addr, usize size);
// void pmp_disable(int idx);

// void pmp_switch(u64 eid);
void activate_lpmp(u64 eid);
__unused void pmp_dump(void);
__unused void __pmp_dump(void);

#endif // EBI_PMP