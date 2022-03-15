#include "page_table.h"
#include "page_pool.h"
#include "memory.h"
#include "../panic/panic.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>

#define NOP	"addi	a0, a0, 0\n\t"

/**
 * @brief Sv39 addressing is used.
 */
__page_aligned
static pte_t page_table_root[PAGE_SIZE / sizeof(pte_t)];

/**
 * @param alloc: Whether allocate pages for sub page table
 */
static pte_t *get_leaf_pte(vaddr_t vaddr, u8 level, u8 alloc)
{
	sv39_vaddr_t va = va_to_sv39(vaddr);

	pte_t *table_root = &page_table_root[0];

	for (int i = level; i > 0; i--) {
		u64 vpn = get_vpn(va, i);
		pte_t *pte = &table_root[vpn];

		paddr_t next_table_pa;
		if (pte->v == 0) {
			if (alloc != GET_PTE_ALLOC)
				goto error;

			next_table_pa 	= alloc_smode_page(1);
			pte->ppn		= next_table_pa >> PAGE_SHIFT;
			pte->v			= 1;
		} else {
			next_table_pa = pte->ppn << PAGE_SHIFT;
		}

		// fixme: access next table when mmu is on
		table_root = (pte_t *)next_table_pa;
	}

	u64 vpn = get_vpn(va, 0);
	pte_t *pte = &table_root[vpn];

	if (pte->v == 0 && alloc != GET_PTE_ALLOC)
		goto error;

	return pte;

error:
	panic("Page not mapped!\n");
	return NULL; // should never reach here
}

__unused static paddr_t get_pa(vaddr_t va)
{
	pte_t pte = *get_leaf_pte(va, SV39_LEVEL_PAGE, GET_PTE_NO_ALLOC);
	show(pte);
	show(pte.ppn);
	return pte.ppn << PAGE_SHIFT;
}

// should be called only once during boot
void turn_on_mmu()
{
	paddr_t page_table_root_addr = (paddr_t)&page_table_root;
	csr_satp_t satp = {
		.mode = SATP_SV39,
		.asid = 0,
		.ppn = page_table_root_addr >> PAGE_SHIFT
	};
	u64 satp_value = *(u64 *)&satp;

	show(page_table_root_addr);
	show(satp_value);

	usize va_pa_offset = get_emod_manager_va_start()
		- get_emod_manager_pa_start();

	// flush_tlb();
	asm volatile (
		"la		s7, temp_stvec	\n\t"	// s7 = &temp_stvec;
		"mv		s6, %0			\n\t"	// s6 = va_pa_offset;
		"add	s7, s6, s7		\n\t"	// s7 += s6;
		"csrw	stvec, s7		\n\t"	// stvec = s7;

		"csrw	satp, %1		\n\t"	// satp = satp_value
	"temp_stvec:				\n\t"
		NOP
		:							// no output register
		: "r"(va_pa_offset), "r"(satp_value)
		: "s6", "s7"
	);
	// flush_tlb();
}

/**
 * @brief map given va to pa
 * 
 * @param vaddr 
 * @param paddr 
 * @param flags: PTE_V by default, PTE_D as alias to PTE_W,
 * 	PTE_A as alias to PTE_R
 * @param level: one of LEVEL_PAGE, LEVEL_MEGA and LEVEL_GIGA
 */
void map_page(vaddr_t vaddr, paddr_t paddr, u8 flags, u8 level)
{
	sv39_paddr_t pa = pa_to_sv39(paddr);
	pte_t *pte = get_leaf_pte(vaddr, level, GET_PTE_ALLOC);

	if (flags & PTE_R)
		pte->r = pte->a = 1;
	if (flags & PTE_W)
		pte->w = pte->d = 1;
	if (flags & PTE_X)
		pte->x = 1;
	if (flags & PTE_U)
		pte->u = 1; // user mode page
	if (flags & PTE_G)
		pte->g = 1;
	pte->v = 1;

	pte->ppn = pa.ppn;

	// debug("va:\t0x%lx\t->\tpa:\t0x%lx\n", vaddr, paddr);
}

// test
void page_table_test()
{
	paddr_t pa_start = get_emod_manager_pa_start();
	vaddr_t va_start = pa_start;

	usize len = PARTITION_SIZE / PAGE_SIZE;
	for (int i = 0; i < len; i++) {
		map_page(va_start, pa_start,
			PTE_R | PTE_W | PTE_X, SV39_LEVEL_PAGE);
	
		va_start += PAGE_SIZE;
		pa_start += PAGE_SIZE;
	}
}