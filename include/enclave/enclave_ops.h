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