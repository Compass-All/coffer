#pragma once

#include <emodules/ecall.h>
#include <types.h>
#include <ebi_ecall.h>

// exit
// mem_alloc
// map_register

static paddr_t inline __ecall_ebi_mem_alloc(usize number_of_partitions)
{
	paddr_t paddr;
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_MEM_ALLOC,
		0UL, number_of_partitions, 0UL
	);
	asm volatile (
		"mv		%0, a1	\n\t"
		: "=r"(paddr)
	);
	return paddr;
}

static void inline __ecall_ebi_addr_record(
	usize 	page_table_offset,
	paddr_t emod_manager_start_pa_ptr,
	paddr_t payload_start_pa_ptr
)
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_ADDR_RECORD,
		page_table_offset,
		emod_manager_start_pa_ptr,
		payload_start_pa_ptr
	);
}

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

static void inline __ecall_ebi_suspend()
{
	__ecall(
		SBI_EXT_EBI,
		SBI_EXT_EBI_SUSPEND,
		0UL, 0UL, 0UL
	);
}