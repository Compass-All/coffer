#pragma once

#include <string.h>
#include <types.h>
#include "../../../dependency.h"

#define SYS_MBOX_NULL   NULL
#define SYS_SEM_NULL    NULL

typedef void * sys_prot_t;
typedef void * sys_sem_t;
typedef void * sys_mbox_t;
typedef void * sys_thread_t;

u32 sys_now();
int atoi(const char *strn);