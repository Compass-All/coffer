#pragma once

#include <emodules/ecall.h>
#include <types.h>
#include <ebi_ecall.h>
#include <enclave/eid.h>

// enter

static u64 inline __ecall_ebi_create_enclave()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_CREATE,
		0UL, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_enter_enclave(
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

// TODO: change return type
static u64 inline __ecall_ebi_resume_enclave(
	u64 enclave_id,
	u64 short_message
)
{
	u64 ret;
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_RESUME,
		enclave_id,
		short_message,
		0UL
	);
	asm volatile (
		"mv		%0, a0	\n\t"
		: "=r"(ret)
	);
	return ret;
}