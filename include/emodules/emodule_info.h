#pragma once

#include <types.h>

typedef struct {
	bool	loaded;

	u32		id;
	vaddr_t	(*get_func_addr)(u32 func_id);
} emodule_info_t;