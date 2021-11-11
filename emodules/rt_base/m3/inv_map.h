#pragma once

#include <util/memory.h>

extern inverse_map_t inv_map[INVERSE_MAP_ENTRY_NUM];

void inv_map_init(void);

inverse_map_t* inv_map_insert(uintptr_t pa, uintptr_t va, uint32_t count);