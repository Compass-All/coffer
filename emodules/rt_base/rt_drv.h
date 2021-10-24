#pragma once

#include <util/drv.h>

typedef drv_ctrl_t* (*drv_initer)(void);

extern drv_ctrl_t* peri_reg_list[MAX_DRV];
extern drv_initer drv_init_list[MAX_DRV];
extern drv_addr_t* drv_addr_list;

void init_drivers(void);