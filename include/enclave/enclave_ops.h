#pragma once

#include <emodules/ecall.h>
#include <types.h>
#include <ebi_ecall.h>

// exit
// mem_alloc
// map_register

static void inline __ecall_ebi_puts(paddr_t string_ptr)
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_PUTS,
		(u64)string_ptr, 0UL, 0UL
	);
}

static void inline __ecall_ebi_exit(u64 exit_val)
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_EXIT,
		exit_val, 0UL, 0UL
	);
}

static int inline __ecall_ebi_suspend()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_SUSPEND,
		0UL, 0UL, 0UL
	);
}