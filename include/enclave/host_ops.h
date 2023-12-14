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
	u64 thread_id,
	u64 short_message
)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_RESUME,
		enclave_id,
        thread_id,
		short_message
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

static void inline __ecall_dump_region()
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_DEBUG_DUMP_REGION,
		0UL, 0UL, 0UL
	);
}

static void inline __ecall_unmatched_acc_fault(u64 addr)
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_DEBUG_UNMATCHED_ACC_FAULT,
		addr, 0UL, 0UL
	);
}

static u64 inline __ecall_get_timer(u64 eid, u64 timer)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EVAL_GET_TIMER,
		eid, timer, 0UL
	);
}

static void inline __ecall_reset_coffer()
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_RESET,
		0UL, 0UL, 0UL
	);
}

static u64 inline __ecall_get_eid_count()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_GET_EID_COUND,
		0UL, 0UL, 0UL
	);
}

static u64 inline __ecall_get_alive_count()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_GET_ALIVE_COUNT,
		0UL, 0UL, 0UL
	);
}

static u64 inline __ecall_get_enclave_status(u64 eid)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_GET_STATUS,
		eid, 0UL, 0UL
	);
}