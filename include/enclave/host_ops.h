#pragma once

#include <emodules/ecall.h>
#include <types.h>
#include <ebi_ecall.h>
#include <enclave/eid.h>

// enter

static u64 inline __ecall_ebi_create()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_CREATE,
		0UL, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_enter(
	u64 	enclave_id,
	u64 	argc,
	vaddr_t argv,
	vaddr_t payload_addr,
	usize 	payload_size

)
{
	return __ecall_5(
		SBI_EXT_EBI,
		SBI_EXT_EBI_ENTER,
		enclave_id,
		argc,
		argv,
		payload_addr,
		payload_size
	);
}

static u64 inline __ecall_ebi_resume(
	u64 enclave_id,
	u64 short_message
)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_RESUME,
		enclave_id,
		short_message,
		0UL
	);
}

static void inline __ecall_dump_enclave_status()
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_DEBUG_DUMP_STATUS,
		0UL, 0UL, 0UL
	);
}

static void inline __ecall_dump_enclave_memory()
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_DEBUG_DUMP_OWNERSHIP,
		0UL, 0UL, 0UL
	);
}

static void inline __ecall_dump_pmp()
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_DEBUG_DUMP_PMP,
		0UL, 0UL, 0UL
	);
}