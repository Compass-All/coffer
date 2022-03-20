/**
 * @file emodule_desc.h
 * @brief emodule descriptor
 */

#pragma once

#include <types.h>

typedef struct {
	u32 	emod_id; // emodule_id.h
	char	name[32];
	u8		__signature; // to do
} emod_desc_t;