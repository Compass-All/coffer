#include <types.h>
#include <emodules/emod_manager/emod_manager.h>
#include <emodules/ecall.h>
#include <ebi_ecall.h>
#include <enclave/enclave_ops.h>
#include <memory/memory.h>
#include <memory/page_table.h>
#include "../elf/elf_loader.h"
#include "../memory/memory.h"
#include "../memory/page_pool.h"
#include "../memory/page_table.h"
#include <util/gnu_attribute.h> 
#include <util/register.h>
#include <util/csr.h>
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
 * 	a0 = id, a1 = base_pa_start, a2 = base_size
 * 
 * TODO:
 * - When should CSRs get initialized?
 */
void emain_upper_half(
	u64 	eid,
	paddr_t emod_manager_pa_start,
	usize	emod_manager_size
)
{
	extern u8 _end; // defined in the linker script
	paddr_t emod_manager_pa_end = (paddr_t)&_end;

	/* upper half of enclave initialization */
	debug("Beginning of emain upper half\n");
	show(eid);
	show(emod_manager_pa_start); show(emod_manager_size);

	set_emod_manager_pa_start(emod_manager_pa_start);
	init_smode_page_pool(
		emod_manager_pa_end - emod_manager_pa_start,
		PAGE_DOWN(
			PARTITION_UP(emod_manager_pa_end)
			- emod_manager_pa_end
		)
	);

	map_smode_page_pool();
	map_sections();
	setup_linear_map();

	u64 satp_value = init_satp();
	usize va_pa_offset = get_va_pa_offset();
	vaddr_t smode_sp = alloc_smode_stack() + va_pa_offset;

	show(satp_value); show(smode_sp); show(va_pa_offset);

	addr_record(satp_value);

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

static void set_csr(u64 sepc, u64 sscratch)
{
	u64 sstatus = read_csr(sstatus);
    sstatus |= SSTATUS_SUM;		// set SUM
    sstatus &= ~SSTATUS_SPP;	// flip SPP
    write_csr(sstatus, sstatus);

	// write_csr(sie, SIE_SEIE | SIE_SSIE);

	write_csr(sepc, sepc);
	write_csr(sscratch, sscratch);
}

void emain_lower_half()
{
	paddr_t	payload_pa_start;
	usize	payload_size;
	u64		argc;

	__ecall_ebi_suspend();
	asm volatile(
		"mv		%0, a0	\n\t"
		"mv		%1, a1	\n\t"
		"mv		%2, a2	\n\t"
		:	"=r"(payload_pa_start),
			"=r"(payload_size),
			"=r"(argc)
	);

	/* lower half of enclave initialization */
	debug("Beginning of emain lower half\n");

	show(payload_pa_start);
	show(payload_size);
	show(argc);

	set_payload_pa_start(payload_pa_start);
	paddr_t payload_pa_end = payload_pa_start + PAGE_UP(payload_size);

	// currently only support less than one page argv
	paddr_t	user_argv_pa_start	= payload_pa_end;
	paddr_t user_argv_pa_end	= payload_pa_end + PAGE_SIZE;
	show(user_argv_pa_start);
	show(user_argv_pa_end);

	map_user_argv(user_argv_pa_start, argc);

	init_umode_page_pool(
		user_argv_pa_end - payload_pa_start,
		PAGE_DOWN(
			PARTITION_UP(user_argv_pa_end) - user_argv_pa_end
		)
	);

	vaddr_t user_argv_va = alloc_map_umode_stack();
	vaddr_t umode_stack_top = user_argv_va - PAGE_SIZE;
	show(user_argv_va);
	show(umode_stack_top);

#define PUSH(value)							\
	{										\
		umode_stack_top -= sizeof(u64);		\
		*(u64 *)umode_stack_top = value;	\
	}
	for (int i = argc - 1; i >= 0; i--) {
		u64 *argv_ptr = (u64 *)user_argv_va;
		PUSH(argv_ptr[i]);
	}
	PUSH(argc);

	vaddr_t elf_entry = load_elf(payload_pa_start, payload_size);
	show(elf_entry);

	init_prog_brk();

	set_csr(elf_entry, umode_stack_top);

	debug("end of emain\n");
}