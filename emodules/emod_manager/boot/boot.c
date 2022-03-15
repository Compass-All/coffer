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
static void upper_half(paddr_t emod_manager_pa_start)
{
	extern u8 _end; // defined in the linker script
	paddr_t emod_manager_pa_end = (paddr_t)&_end;

	/* upper half of enclave initialization */

	set_emod_manager_pa_start(emod_manager_pa_start);
	init_page_pool(emod_manager_pa_end - emod_manager_pa_start);
	map_sections();
	turn_on_mmu();

	panic("End of upper half\n");
}

static void lower_half()
{
	/* lower half of enclave initialization */
	
	// enter/resume

	emod_manager_init();
	emod_manager_test();

	panic("Test panic\n");
}

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
void emain(
	u64 	eid,
	paddr_t emod_manager_pa_start,
	usize	emod_manager_size,
	paddr_t	payload_pa_start,
	usize	payload_size
)
{
	debug("Hello world, I am enclave %lu\n", eid);

	upper_half(emod_manager_pa_start);
	__ecall_ebi_suspend();
	panic("Before lower half\n");
	lower_half();
}
