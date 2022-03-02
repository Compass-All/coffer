// TODO (this file should be depressed considering the new design of device tree)
#ifndef EBI_DRV_H
#define EBI_DRV_H

#include "util.h"

#define MAX_DRV 64
#define CMD_QUERY_INFO -1

#ifndef __ASSEMBLER__

typedef uintptr_t (*cmd_handler)(uintptr_t, uintptr_t, uintptr_t, uintptr_t);

typedef struct {
    uintptr_t reg_addr;
    uintptr_t reg_size;
} drv_ctrl_t;

#endif // __ASSEMBLER__
#endif // EBI_DRV_H
