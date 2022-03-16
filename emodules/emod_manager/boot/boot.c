#include <types.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/ecall.h>
#include <ebi_ecall.h>
#include <enclave/enclave_ops.h>
#include <memory/memory.h>
#include <memory/page_table.h>
#include "../memory/memory.h"
#include "../memory/page_pool.h"
#include "../memory/page_table.h"
#include <util/gnu_attribute.h> 
#include "../printf/printf.h"
#include "../debug/debug.h"
#include "../panic/panic.h"

// temporary stack for enclave boot
#define TMP_STACK_SIZE	0x1000
u8 tmp_stack[TMP_STACK_SIZE];
void *const tmp_stack_top = (void *)tmp_stack + TMP_STACK_SIZE;

// ---------------
/**
 * @brief Entry point of the enclave
 * 
 * The initialization of the enclave is divided into two sections:
 * the upper half and the lower half. The two sections are divided by
 * an ebi_suspend ecall.
 * 
 * In the upper half, the page table and inverse memory map are set up
 * so that the enclave is ready to be migrated passively.
 * 
 * In the lower half, a basic page pool gets initialized, the ELF payload
 * gets loaded and the emodule manager gets initialized.
 * 
 * After initialization, the control flow is transferred to the U mode
 * payload.
 * 
 * @params:
 * 	a0 = id, a1 = base_pa_start, a2 = base_size, 
 * 	a3 = payload_pa_start, a4 = payload_size
 * 
 * TODO:
 * - When should CSRs get initialized?
 */
void emain_upper_half(
	u64 	eid,
	paddr_t emod_manager_pa_start,
	usize	emod_manager_size,
	paddr_t	payload_pa_start,
	usize	payload_size
)
{
	extern u8 _end; // defined in the linker script
	paddr_t emod_manager_pa_end = (paddr_t)&_end;

	/* upper half of enclave initialization */

	set_emod_manager_pa_start(emod_manager_pa_start);
	init_page_pool(
		emod_manager_pa_end - emod_manager_pa_start,
		PAGE_DOWN(
			PARTITION_UP(emod_manager_pa_end)
			- emod_manager_pa_end
		)
	);

	map_page_pool();
	map_sections();

	u64 satp_value = init_satp();
	usize va_pa_offset = get_va_pa_offset();
	vaddr_t smode_sp = alloc_smode_stack() + va_pa_offset;

	show(satp_value); show(smode_sp); show(va_pa_offset);

	asm volatile(
		"mv		a0, %0	\n\t"
		"mv		a1, %1	\n\t"
		"mv 	s6, %2	\n\t"
		:
		:	"r"(satp_value),
			"r"(smode_sp),
			"r"(va_pa_offset)
		// :	"a0", "a1", "s6" DO NOT UNCOMMENT THIS LINE!
	);
}

void emain_lower_half()
{
	__ecall_ebi_suspend(); // this line gets executed

	// panic("Beginning of lower half\n");

	/* lower half of enclave initialization */
	
	// enter/resume

	emod_manager_init();
	emod_manager_test();

	panic("Test panic\n");
}