#include "page_table.h"
#include "page_pool.h"
#include "memory.h"
#include "../panic/panic.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>
#include <util/register.h>

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

	if (va.extension == 0)
		show(table_root);

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

		table_root = (pte_t *)next_table_pa;
		if (read_csr(satp))
			table_root += get_va_pa_offset();
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

__unused paddr_t get_pa(vaddr_t va)
{
	pte_t pte = *get_leaf_pte(va, SV39_LEVEL_PAGE, GET_PTE_NO_ALLOC);
	return pte.ppn << PAGE_SHIFT;
}

u64 init_satp()
{
	paddr_t page_table_root_addr = (paddr_t)&page_table_root;
	csr_satp_t satp = {
		.mode = SATP_SV39,
		.asid = 0,
		.ppn = page_table_root_addr >> PAGE_SHIFT
	};
	u64 satp_value = *(u64 *)&satp;

	return satp_value;
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

	// TODO: flush_tlb of the page
}


// ---------------
// test
void page_table_test()
{
	vaddr_t root_addr;
	asm volatile(
		"la	t1, page_table_root	\n\t"
		"mv	%0, t1				\n\t"
		: "=r"(root_addr)
		:
		: "t1"
	);
	show(root_addr);
}