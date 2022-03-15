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

static void dump_addr()
{
	__unused extern u8 _start, _end;
	__unused extern u8 _text_start, _text_end;
	__unused extern u8 _init_data_start, _init_data_end;
	__unused extern u8 _rodata_start, _rodata_end;
	__unused extern u8 _srodata_start, _srodata_end;
	__unused extern u8 _data_start, _data_end;
	__unused extern u8 _bss_start, _bss_end;

	// addr dump
	show(&_start); show(&_end);
	show(&_text_start); show(&_text_end);
	show(&_init_data_start); show(&_init_data_end);
	show(&_rodata_start); show(&_rodata_end);
	show(&_srodata_start); show(&_srodata_end);
	show(&_data_start); show(&_data_end);
	show(&_bss_start); show(&_bss_end);

	__unused paddr_t partition_end = PARTITION_UP((paddr_t)&_end);
	show(partition_end);
	
	__unused usize left_space_size = partition_end - (paddr_t)&_end;
	show(left_space_size);
}

__unused static void map_emod_manager()
{
	todo();
	return;
}

__unused static void upper_half()
{
	todo();
	return;
}

__unused static void lower_half()
{
	todo();
	return;
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
	extern u8 _end; // defined in the linker script
	paddr_t emod_manager_pa_end = (paddr_t)&_end;

	/* upper half of enclave initialization */
	debug("hello world\n");
	dump_addr();

	set_emod_manager_pa_start(emod_manager_pa_start);
	init_page_pool(emod_manager_pa_end - emod_manager_pa_start);

	page_table_test();

	panic("upper half finished\n");
	// ---------------
	__ecall_ebi_suspend();

	/* lower half of enclave initialization */

	// enter/resume

	emod_manager_init();
	emod_manager_test();

	panic("Test panic\n");
}
