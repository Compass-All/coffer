#pragma once

#include <string.h>
#include <types.h>
#include "../../../dependency.h"

#define SYS_MBOX_NULL   NULL
#define SYS_SEM_NULL    NULL

// typedef void * sys_prot_t;
// typedef void * sys_sem_t;
// typedef void * sys_mbox_t;
// typedef void * sys_thread_t;

struct uk_alloc {
    char _dummy;
};

u32 sys_now();
int atoi(const char *strn);
paddr_t ukplat_virt_to_phys(const void *vaddr);

#define local_irq_save(x)	    (x) //__save_and_disable_irq(x)
#define local_irq_restore(x)	(x) //__restore_flags(x)

uint32_t gic_irq_translate(uint32_t type, uint32_t irq);

void free(void *ptr);
void *malloc(size_t size);
void *calloc(size_t count, size_t size);
void *memalign(size_t alignment, size_t size);
#define zalloc(size) calloc(1, (size))