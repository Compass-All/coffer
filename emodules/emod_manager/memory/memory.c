#include "memory.h"
#include "page_table.h"
#include "page_pool.h"
#include "../debug/debug.h"
#include "../panic/panic.h"

// initialized during boot
volatile 	static paddr_t emod_manager_pa_start;
// defined in config.mk
const		static vaddr_t emod_manager_va_start = EMOD_MANAGER_VA_START;

// program break, to be initialized
static vaddr_t prog_brk = 0;

void wait_until_non_zero(volatile u64 *ptr)
{
	while (*ptr == 0UL);
}

void set_emod_manager_pa_start(paddr_t pa_start)
{
	emod_manager_pa_start = pa_start;
}

paddr_t get_emod_manager_pa_start()
{
	return emod_manager_pa_start;
}

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
#define EUSR_HEAP_START_ALIGNED		0x400000000UL
void init_prog_brk()
{
	usize u_mode_pool_remain_size = get_umode_page_pool_avail_size();
	set_prog_brk(EUSR_HEAP_START_ALIGNED - u_mode_pool_remain_size);

	show(get_prog_brk());
}

static void __map_section(memory_section_t mem_sec)
{
	paddr_t paddr = mem_sec.offset + get_emod_manager_pa_start();
	vaddr_t vaddr = mem_sec.vaddr;
	usize number_of_pages = PAGE_UP(mem_sec.size) / PAGE_SIZE;

	if (mem_sec.offset % PAGE_SIZE != 0)
		panic("Section not aligned!\n");

	for (int i = 0; i < number_of_pages; i++) {
		map_page(vaddr, paddr, mem_sec.flags, SV39_LEVEL_PAGE);

		vaddr += PAGE_SIZE;
		paddr += PAGE_SIZE;
	}
}

void map_page_pool()
{
	paddr_t page_pool_start_pa = get_page_pool_offset()
		+ get_emod_manager_pa_start();
	usize page_pool_size = get_page_pool_size();

	show(page_pool_start_pa);
	show(page_pool_size);

	for (int i = 0; i < page_pool_size / PAGE_SIZE; i++) {
		map_page(
			page_pool_start_pa + i * PAGE_SIZE + get_va_pa_offset(),
			page_pool_start_pa + i * PAGE_SIZE,
			PTE_R | PTE_W,
			SV39_LEVEL_PAGE
		);
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

#define SMODE_STACK_SIZE	0x10000
#define UMODE_STACK_SIZE	0x10000

#define UMODE_STACK_TOP_VA	0x800000000UL // to be considered

paddr_t alloc_smode_stack()
{
	debug("allocating S mode stack: %lu pages\n",
		SMODE_STACK_SIZE / PAGE_SIZE);
	return alloc_smode_page(SMODE_STACK_SIZE / PAGE_SIZE)
		+ SMODE_STACK_SIZE;
}

// allocate user mode stack, return stack top va
vaddr_t alloc_map_umode_stack()
{
	usize number_of_pages = UMODE_STACK_SIZE / PAGE_SIZE;
	debug("allocating U mode stack: %lu pages\n",
		number_of_pages);

	paddr_t umode_stack_bottom_paddr =
		alloc_umode_page(number_of_pages);
	vaddr_t umode_stack_bottom_vaddr =
		UMODE_STACK_TOP_VA - UMODE_STACK_SIZE;

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

		if (aligned_new_brk > aligned_old_brk) {
			usize alloc_size	 			= aligned_new_brk - aligned_old_brk;
			usize remained_umode_pool_size	= get_umode_page_pool_avail_size();
			show(alloc_size);
			show(remained_umode_pool_size);

			if (alloc_size <= remained_umode_pool_size) {
				usize number_of_pages = alloc_size / PAGE_SIZE;
				paddr_t paddr = alloc_umode_page(number_of_pages);
				for (int i = 0; i < number_of_pages; i++) {
					map_page(
						aligned_old_brk		+ i * PAGE_SIZE,
						paddr				+ i * PAGE_SIZE,
						PTE_U | PTE_W | PTE_R,
						SV39_LEVEL_PAGE
					);
				}
			} else {
				show(remained_umode_pool_size);
				panic("Out of memory\n");

				// TODO: allocate from SM
			}
		}

		set_prog_brk(new_brk);
		return new_brk;
	}

	
}