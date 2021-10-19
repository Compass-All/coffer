#ifndef EBI_PMP_H
#define EBI_PMP_H

#include "enclave.h"
#include "memory.h"

void pmp_switch(enclave_context_t *context);
void pmp_update(enclave_context_t *context);
void pmp_allow_access(peri_addr_t *peri);
void pmp_allow_region(uintptr_t pa, uintptr_t size);
void pmp_withdraw(section_t *sec);

#endif // EBI_PMP_H