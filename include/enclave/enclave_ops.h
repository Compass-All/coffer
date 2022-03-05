#pragma once

#include <emodules/ecall.h>
#include <types.h>
#include <ebi_ecall.h>

// exit
// mem_alloc
// map_register

static int inline __ecall_ebi_suspend()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_SUSPEND,
		0UL, 0UL, 0UL
	);
}