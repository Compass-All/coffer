#include "rt_drv.h"

drv_ctrl_t* peri_reg_list[MAX_DRV] = { 0 };
drv_initer drv_init_list[MAX_DRV];
drv_addr_t* drv_addr_list;

void init_drivers(void)
{
}