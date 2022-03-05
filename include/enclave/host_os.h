#pragma once

#include <emodules/ecall.h>
#include <types.h>
#include <ebi_ecall.h>

#define HOST_EID	0UL

// create
// enter

// TODO: change return type
static void inline __ecall_ebi_resume(u64 enclave_id)
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_RESUME,
		enclave_id, 0UL, 0UL
	);
}