#pragma once

#include <emodules/ecall.h>
#include <types.h>
#include <ebi_ecall.h>

// exit
// mem_alloc
// map_register

static paddr_t inline __ecall_ebi_mem_alloc(
	usize number_of_partitions,
	usize *suggestion
)
{
	paddr_t paddr;
	usize ret;
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_MEM_ALLOC,
		0UL, number_of_partitions, 0UL
	);
	asm volatile (
		"mv		%0, a0	\n\t"
		"mv		%1, a1	\n\t"
		: "=r"(ret), "=r"(paddr)
		:
		: "a0", "a1", "memory"
	);
	if (suggestion)
		*suggestion = ret;

	return paddr;
}

// static void inline __ecall_ebi_addr_record(
// 	paddr_t emod_manager_start_pa_ptr,
// 	paddr_t umode_pool_pa_aligned_ptr
// )
// {
// 	__ecall(
// 		SBI_EXT_EBI,
// 		SBI_EXT_EBI_ADDR_RECORD,
// 		emod_manager_start_pa_ptr,
// 		umode_pool_pa_aligned_ptr,
// 		0UL
// 	);
// }

static void inline __ecall_ebi_exit(u64 exit_val)
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_EXIT,
		exit_val, 0UL, 0UL
	);
}

static void inline __ecall_ebi_exit_thread(u64 exit_val)
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_EXIT_THREAD,
		exit_val, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_block_thread(u64 tid)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_BLOCK_THREAD,
		tid, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_unblock_threads(u64 threads)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_UNBLOCK_THREADS,
		threads, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_get_blocked_threads()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_GET_BLOCKED_THREADS,
		0UL, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_set_clear_child_tid(int *tidptr)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_GET_CLEAR_CHILD_TID,
		(u64)tidptr, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_get_clear_child_tid()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_GET_CLEAR_CHILD_TID,
		0UL, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_suspend(u64 short_message)
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_SUSPEND,
		short_message, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_get_eid()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_GET_EID,
		0UL, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_get_tid()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_GET_TID,
		0UL, 0UL, 0UL
	);
}

static u64 inline __ecall_ebi_get_hartid()
{
	return __ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_GET_HARTID,
		0UL, 0UL, 0UL
	);
}
