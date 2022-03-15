#include "page_table.h"
#include "page_pool.h"
#include "../panic/panic.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>


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
		show(next_table_pa);

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

	if (flags | PTE_R)
		pte->r = pte->a = 1;
	if (flags | PTE_W)
		pte->w = pte->d = 1;
	if (flags | PTE_X)
		pte->x = 1;
	if (flags | PTE_U)
		pte->u = 1; // user mode page
	if (flags | PTE_G)
		pte->g = 1;
	pte->v = 1;

	pte->ppn = pa.ppn;
}

static paddr_t get_pa(vaddr_t va)
{
	pte_t pte = *get_leaf_pte(va, SV39_LEVEL_PAGE, GET_PTE_NO_ALLOC);
	show(pte);
	show(pte.ppn);
	return pte.ppn << PAGE_SHIFT;
}

// test
void page_table_test()
{
	show(&page_table_root);

	vaddr_t va = EMOD_MANAGER_SVA_START;
	sv39_vaddr_t sv39_va = va_to_sv39(va);

	show(va);
	show(sv39_va.offset);
	show(sv39_va.vpn0);
	show(sv39_va.vpn1);
	show(sv39_va.vpn2);
	show(sv39_va.extension);

	map_page(0x10000, 0x14c414000,
		PTE_W | PTE_R, SV39_LEVEL_PAGE);

	paddr_t pa = get_pa(0x10000);
	show(pa);

	pa = get_pa(0x11000);
	show(pa);
}