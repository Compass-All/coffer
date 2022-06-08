#include "memory.h"
#include "page_table.h"
#include "page_pool.h"
#include "../debug/debug.h"
#include "../panic/panic.h"
#include <enclave/enclave_ops.h>
#include <util/register.h>

// TODO: record &emod_pa_start and &elf_pa_start to m mode

// initialized during creating enclave
volatile static	paddr_t emod_manager_pa_start;
// defined in config.mk
const static	vaddr_t emod_manager_va_start = EMOD_MANAGER_VA_START;

// initialized during entering enclave
volatile static paddr_t payload_pa_start;

// for emodules va allocation
static 			vaddr_t emodule_brk = EMOD_INIT_BRK;

// program break, to be initialized
static vaddr_t prog_brk = 0;

void wait_until_non_zero(volatile u64 *ptr)
{
	while (*ptr == 0UL);
}

void set_emod_manager_pa_start(paddr_t pa_start)
{
	emod_manager_pa_start = pa_start;
	show(emod_manager_pa_start);
}

paddr_t get_emod_manager_pa_start()
{
	u64 satp_value = read_csr(satp);
	csr_satp_t satp = *(csr_satp_t *)&satp_value;
	if (satp_value)
		return PARTITION_DOWN(satp.ppn << PAGE_SHIFT);
	else
		return emod_manager_pa_start;
}

void set_payload_pa_start(paddr_t pa_start)
{
	payload_pa_start = pa_start;
	show(payload_pa_start);
}

paddr_t get_payload_pa_start()
{
	return payload_pa_start;
}

static void increase_emodule_brk(usize increment)
{
	emodule_brk += increment;
}

static vaddr_t get_emodule_brk()
{
	return emodule_brk;
}

vaddr_t alloc_map_emodule(usize emodule_size)
{
	usize	alloc_size		= PAGE_UP(emodule_size);
	usize	number_of_pages	= alloc_size >> PAGE_SHIFT;
	paddr_t	paddr			= alloc_smode_page(number_of_pages);
	vaddr_t vaddr			= get_emodule_brk();

	for (int i = 0; i < number_of_pages; i++) {
		map_page(
			vaddr + i * PAGE_SIZE,
			paddr + i * PAGE_SIZE,
			PTE_R | PTE_W | PTE_X,
			SV39_LEVEL_PAGE
		);
	}

	increase_emodule_brk(alloc_size);

	return vaddr;
}

// this function should not be used after
// the enclave gets initialized
usize get_va_pa_offset()
{
	return emod_manager_va_start - emod_manager_pa_start;
}

static inline vaddr_t get_prog_brk()
{
	return prog_brk;
}

static inline void set_prog_brk(vaddr_t new_brk)
{
	prog_brk = new_brk;
}

// prog_brk is initialized to
// EUSR_HEAP_START_ALIGNED - (available U mode page size)
// invoke this only after initialize U mode page pool!
void init_prog_brk()
{
	usize u_mode_pool_remain_size = get_umode_page_pool_avail_size();
	set_prog_brk(EUSR_HEAP_START_ALIGNED - u_mode_pool_remain_size);

	show(get_prog_brk());
}

void addr_record(u64 satp_value)
{
	paddr_t page_table_root = satp_value << PAGE_SHIFT;
	usize page_table_offset = page_table_root - get_emod_manager_pa_start();
	__ecall_ebi_addr_record(
		page_table_offset,
		(paddr_t)&emod_manager_pa_start,
		(paddr_t)&payload_pa_start
	);
}

static void __map_section(memory_section_t mem_sec)
{
	paddr_t paddr = mem_sec.offset + get_emod_manager_pa_start();
	vaddr_t vaddr = mem_sec.vaddr;
	usize number_of_pages = PAGE_UP(mem_sec.size) >> PAGE_SHIFT;

	show(mem_sec.offset);
	show(mem_sec.size);
	show(mem_sec.vaddr);
	show(mem_sec.flags);

	if (mem_sec.offset % PAGE_SIZE != 0)
		panic("Section not aligned!\n");

	for (int i = 0; i < number_of_pages; i++) {
		map_page(vaddr, paddr, mem_sec.flags, SV39_LEVEL_PAGE);

		vaddr += PAGE_SIZE;
		paddr += PAGE_SIZE;
	}
}

// should be called only once during boot
void map_sections()
{
	paddr_t start_pa = get_emod_manager_pa_start();
	vaddr_t start_va = start_pa + get_va_pa_offset();

#define MAP_SECTION(name, sec_flags) 									\
	{																	\
		extern u8	_##name##_start, _##name##_end;						\
		usize offset 	= (usize)(&_##name##_start - start_pa);			\
		usize size 		= (usize)(&_##name##_end - &_##name##_start);	\
		vaddr_t	vaddr	= offset + start_va;							\
		memory_section_t section = {	\
			.offset	= offset,			\
			.size	= size,				\
			.vaddr	= vaddr,			\
			.flags	= sec_flags			\
		};								\
		debug(#name":\n");				\
		show(section.offset);			\
		show(section.size);				\
		show(section.vaddr);			\
		show(section.flags);			\
		__map_section(section);			\
	}

	MAP_SECTION(text, 		PTE_X);
	MAP_SECTION(init_data, 	PTE_R | PTE_W);
	MAP_SECTION(rodata, 	PTE_R);
	MAP_SECTION(srodata, 	PTE_R);
	MAP_SECTION(data, 		PTE_R | PTE_W);
	MAP_SECTION(bss, 		PTE_R | PTE_W);
}

paddr_t alloc_smode_stack()
{
	debug("allocating S mode stack: %lu pages\n",
		SMODE_STACK_SIZE >> PAGE_SHIFT);
	return alloc_smode_page(SMODE_STACK_SIZE >> PAGE_SHIFT)
		+ SMODE_STACK_SIZE;
}

void map_user_argv(paddr_t user_argv_pa, u64 argc)
{
	vaddr_t user_argv_va = UMODE_STACK_TOP_VA;

	u64 *user_argv_ptr = (u64 *)(user_argv_pa + linear_map_offset);
	for (int i = 0; i < argc; i++) {
		u64 offset = user_argv_ptr[i] % PAGE_SIZE;
		user_argv_ptr[i] = user_argv_va + offset;

		show(i);
		show(user_argv_ptr[i]);
		show(offset);
	}

	map_page(
		user_argv_va,
		user_argv_pa,
		PTE_U | PTE_R | PTE_W,
		SV39_LEVEL_PAGE
	);
	show(user_argv_va);
	show(user_argv_pa);
}

// allocate user mode stack, return stack top va
vaddr_t alloc_map_umode_stack()
{
	usize number_of_pages = UMODE_STACK_SIZE >> PAGE_SHIFT;
	debug("allocating U mode stack: %lu pages\n",
		number_of_pages);

	paddr_t umode_stack_bottom_paddr =
		alloc_umode_page(number_of_pages);
	vaddr_t umode_stack_bottom_vaddr =
		UMODE_STACK_TOP_VA - UMODE_STACK_SIZE;

	show(umode_stack_bottom_paddr);
	show(umode_stack_bottom_vaddr);

	for (int i = 0; i < number_of_pages; i++) {
		map_page(
			umode_stack_bottom_vaddr + i * PAGE_SIZE,
			umode_stack_bottom_paddr + i * PAGE_SIZE,
			PTE_U | PTE_R | PTE_W,
			SV39_LEVEL_PAGE
		);
	}

	return (vaddr_t)UMODE_STACK_TOP_VA;
}

// invoked before simple pool out of memory
static void map_brk_from_pool(vaddr_t aligned_old_brk, usize size)
{
	usize number_of_pages = size >> PAGE_SHIFT;
	paddr_t paddr = alloc_umode_page(number_of_pages);
	for (int i = 0; i < number_of_pages; i++) {
		map_page(
			aligned_old_brk		+ i * PAGE_SIZE,
			paddr				+ i * PAGE_SIZE,
			PTE_U | PTE_W | PTE_R,
			SV39_LEVEL_PAGE
		);
	}
}

static paddr_t alloc_partition_from_mmode(usize number_of_partitions)
{
	paddr_t allocated_paddr = __ecall_ebi_mem_alloc(number_of_partitions);
	show(allocated_paddr);

	return allocated_paddr;
}

// invoked after simple pool out of memory, allocating memory from the SM
// size must be parition aligned
static void alloc_map_brk_outside_pool(
	vaddr_t partition_aligned_old_brk,
	usize size
)
{
	usize number_of_partitions = size / PARTITION_SIZE;
	show(number_of_partitions);

	if (size % PARTITION_SIZE)
		panic("size is not aligned\n");

	paddr_t paddr = alloc_partition_from_mmode(number_of_partitions);
	for (int i = 0; i < number_of_partitions; i++) {
		map_page(
			partition_aligned_old_brk	+ i * PARTITION_SIZE,
			paddr						+ i * PARTITION_SIZE,
			PTE_U | PTE_W | PTE_R,
			SV39_LEVEL_MEGA
		);
	}
}

u64 sys_brk_handler(vaddr_t new_brk)
{
	debug("syscall brk handler\n");
	vaddr_t old_brk = get_prog_brk();

	show(old_brk);
	show(new_brk);

	if (new_brk == 0) { // new_brk == 0, querying program break
		debug("querying program break addr\n");
		return old_brk;
	} else { // new_brk != 0, update prog_brk
		debug("trying to grow program break from 0x%lx to 0x%lx\n",
			old_brk, new_brk);

		vaddr_t aligned_new_brk	= PAGE_UP(new_brk);
		vaddr_t aligned_old_brk = PAGE_UP(old_brk);
		show(aligned_old_brk);
		show(aligned_new_brk);

		if (aligned_new_brk > aligned_old_brk) {
			usize remained_umode_pool_size	= get_umode_page_pool_avail_size();
			usize required_size				= aligned_new_brk - aligned_old_brk;
			show(remained_umode_pool_size);
			show(required_size);

			if (remained_umode_pool_size > 0) {
				if (required_size <= remained_umode_pool_size) {
					map_brk_from_pool(aligned_old_brk, required_size);
				} else {
					// use up the rest and update old brk for further allocation
					map_brk_from_pool(aligned_old_brk, remained_umode_pool_size);

					old_brk 		= aligned_old_brk + remained_umode_pool_size;
					aligned_old_brk = old_brk;

					show(get_umode_page_pool_avail_size());
					debug("Pool used up. Temporary old_brk set to 0x%lx\n", old_brk);
				}
			}

			vaddr_t partition_aligned_new_brk = PARTITION_UP(aligned_new_brk);
			vaddr_t partition_aligned_old_brk = PARTITION_UP(aligned_old_brk);
			show(partition_aligned_new_brk);
			show(partition_aligned_old_brk);

			if (partition_aligned_new_brk > partition_aligned_old_brk) {
				usize required_partition_size =
					partition_aligned_new_brk - partition_aligned_old_brk;
				alloc_map_brk_outside_pool(
					partition_aligned_old_brk,
					required_partition_size
				);
			}
		}

		set_prog_brk(new_brk);
		return new_brk;
	}
}