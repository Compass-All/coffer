#pragma once
#include <driver/driver.h>

extern extra_module_t extra_modules[NUM_EXTRA_MODULES];

void probe_extra_modules();
extra_module_t *get_emod_by_id(uint32_t mod_id);