#pragma once

#include <emodules/ecall.h>
#include <types.h>
#include <ebi_ecall.h>

#define HOST_EID	0UL

// enter

static u64 inline __ecall_ebi_create_enclave(
	vaddr_t payload_addr,
	usize payload_len
)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_CREATE,
		(u64)payload_addr,
		(u64)payload_len,
		0UL
	);
}

static u64 inline __ecall_ebi_enter_enclave(
	u64 enclave_id,
	u64 argc,
	vaddr_t argv
)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_ENTER,
		enclave_id,
		argc,
		(u64)argv
	);
}

// TODO: change return type
static void inline __ecall_ebi_resume(u64 enclave_id)
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_RESUME,
		enclave_id, 0UL, 0UL
	);
}