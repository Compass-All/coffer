#pragma once

#include <types.h>

typedef struct {
	bool	loaded;

	u32		id;
	vaddr_t	(*get_func_addr)(u32 func_id);
} emodule_info_t;

#define EMODULE_MANAGER_ID		0
#define EMODULE_DUMMY			1