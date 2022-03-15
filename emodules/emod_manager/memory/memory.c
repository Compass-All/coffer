#include "memory.h"
#include "page_table.h"
#include "../debug/debug.h"
#include "../panic/panic.h"

// initialized during boot
volatile 	static paddr_t emod_manager_pa_start;
// defined in config.mk
const 		static vaddr_t emod_manager_va_start = EMOD_MANAGER_SVA_START;

void set_emod_manager_pa_start(paddr_t pa_start)
{
	emod_manager_pa_start = pa_start;
}

paddr_t get_emod_manager_pa_start()
{
	return emod_manager_pa_start;
}

vaddr_t get_emod_manager_va_start()
{
	return emod_manager_va_start;
}

void wait_until_non_zero(volatile u64 *ptr)
{
	while (*ptr == 0UL);
}

static void __map_section(memory_section_t mem_sec)
{
	paddr_t paddr = mem_sec.offset + get_emod_manager_pa_start();
	vaddr_t vaddr = mem_sec.vaddr;
	usize number_of_pages = mem_sec.size / PAGE_SIZE;

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
	vaddr_t start_va = get_emod_manager_va_start();

#define MAP_SECTION(name, sec_flags) {								\
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