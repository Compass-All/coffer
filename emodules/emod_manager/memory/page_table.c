#include "page_table.h"
#include "page_pool.h"
#include "memory.h"
#include "../panic/panic.h"
#include "../debug/debug.h"
#include <util/gnu_attribute.h>
#include <util/register.h>

#define NOP	"addi	a0, a0, 0\n\t"

const paddr_t linear_map_start	= EMOD_MANAGER_LINEAR_START;
const paddr_t linear_map_size	= EMOD_MANAGER_LINEAR_SIZE;
const paddr_t linear_map_offset	= EMOD_MANAGER_LINEAR_OFFSET;

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

	for (int i = 2; i > 2 - level; i--) {
		u64 vpn = get_vpn(va, i);
		pte_t *pte = &table_root[vpn];

		if (alloc == 0 && level == SV39_LEVEL_MEGA) {
			show(*pte);
			show(pte->ppn);
		}

		paddr_t next_table_pa;
		if (pte->v == 0) {
			if (alloc != GET_PTE_ALLOC)
				goto error;

			next_table_pa 	= alloc_smode_page(1);
			pte->ppn		= next_table_pa >> PAGE_SHIFT;
			pte->v			= 1;
		} else {
			next_table_pa = (u64)pte->ppn << PAGE_SHIFT;
		}

		table_root = (pte_t *)next_table_pa;
		if (read_csr(satp))
			table_root += linear_map_offset / sizeof(pte_t);
	}

	u64 vpn = get_vpn(va, 2 - level);
	pte_t *pte = &table_root[vpn];

	if (pte->v == 0 && alloc != GET_PTE_ALLOC)
		goto error;
	
	return pte;

error:
	panic("Page not mapped!\n");
	return NULL; // should never reach here
}

// invoked before mmu turned on
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
		pte->r = 1;
	if (flags & PTE_W)
		pte->w = 1;
	if (flags & PTE_X)
		pte->x = 1;
	if (flags & PTE_U)
		pte->u = 1; // user mode page
	if (flags & PTE_G)
		pte->g = 1;
	pte->v = 1;

	// always set to 1 if no swap space is used
	pte->a = 1;
	pte->d = 1;

	pte->ppn = (u64)pa.ppn;

	// flush_tlb of the page
	asm volatile(
		"sfence.vma	%0, zero	\n\t" // 'zero' cannot be omitted
		:
		: "r"(vaddr)
	);
}

void setup_linear_map()
{
	paddr_t paddr = linear_map_start;
	for (int i = 0; i < linear_map_size / PARTITION_SIZE; i++) {
		vaddr_t vaddr = paddr + linear_map_offset;
		map_page(vaddr, paddr, PTE_R | PTE_W, SV39_LEVEL_MEGA);

		paddr += PARTITION_SIZE;
	}

	flush_tlb();
}

struct walk_page_table_result {
	u8 		level;	// -1 for not valid
	pte_t	pte;
};

static struct walk_page_table_result walk_page_table(vaddr_t va)
{
#define MASK_OFFSET 0xfff
#define MASK_L0 0x1ff000
#define MASK_L1 0x3fe00000
#define MASK_L2 0x7fc0000000
    u64 layer_offset[] = { (va & MASK_L2) >> 30, (va & MASK_L1) >> 21,
        (va & MASK_L0) >> 12 };
    pte_t* tmp_entry;
	pte_t* root = &page_table_root[0];
	struct walk_page_table_result ret = {
		.level = -1,
	};
    int i;

    for (i = 0; i < 3; ++i) {
        tmp_entry = &root[layer_offset[i]];
        if (!tmp_entry->v) {
			printf("not valid\n");
            return ret;
        }
        if ((tmp_entry->r | tmp_entry->w | tmp_entry->x)) {
			ret.level = i;
			ret.pte = *tmp_entry;
            return ret;
        }

        root = (pte_t*)((u64)tmp_entry->ppn << PAGE_SHIFT);
		if (read_csr(satp)) {
			root += linear_map_offset / sizeof(pte_t);
		}
    }
    return ret;
}

// TODO: consider level here
paddr_t get_pa(vaddr_t va)
{
	struct walk_page_table_result result = walk_page_table(va);
	if (result.level == (u8)-1) {
		printf("VA not valid\n");
		return 0UL;
	}
	u8 level = result.level;
	pte_t pte = result.pte;

	paddr_t base = pte.ppn << PAGE_SHIFT;

	u8 number_of_ones = PAGE_SHIFT + (2 - level) * SV39_VPN_LEN;
	u64 mask = (1 << number_of_ones) - 1;
	u64 offset = mask & va;

	paddr_t pa = base + offset;

	return pa;
}

static void show_pte(vaddr_t va, pte_t pte, u8 level)
{
	if (level > 2)
		panic("invalid level\n");

	__unused paddr_t	pa = pte.ppn << PAGE_SHIFT;
	__unused usize		size;

	if (level == SV39_LEVEL_PAGE)
		size = 0x1000;
	else if (level == SV39_LEVEL_MEGA)
		size = 0x200000;
	else
		size = 0x40000000;

	debug(
		"VA: 0x%lx --> PA: 0x%lx, size: 0x%lx, "
		"R: %d, W: %d, X: %d, U: %d, A: %d, D: %d, G: %d, V: %d\n",
		va, pa, size,
		pte.r, pte.w, pte.x, pte.u, pte.a, pte.d, pte.g, pte.v
	);
}

static void __dump_page_table(u8 level, vaddr_t table_va)
{
	if (level > 2)
		panic("invalid level\n");

#define NUM_ENTRY_PER_TABLE	(PAGE_SIZE / sizeof(pte_t))
	pte_t* root = (pte_t *)table_va;

	sv39_vaddr_t vaddr = {
		.extension = 0,
		.offset = 0,
		.vpn0 = 0,
		.vpn1 = 0,
		.vpn2 = 0
	};

	for (int i = 0; i < NUM_ENTRY_PER_TABLE; i++) {
		pte_t pte = root[i];

		if (!pte.v)
			continue;

		if (level == SV39_LEVEL_PAGE)
			vaddr.vpn0 = i;
		else if (level == SV39_LEVEL_MEGA)
			vaddr.vpn1 = i;
		else
			vaddr.vpn2 = i;

		if (pte.r | pte.w | pte.x) { // leaf pte
			vaddr_t va = sv39_to_va(vaddr);
			show_pte(va, pte, level);
		} else {
			paddr_t next_pa = pte.ppn << PAGE_SHIFT;
			__dump_page_table(
				level - 1,
				(vaddr_t)(next_pa + linear_map_offset)
			);
		}
	}
}

__unused void dump_page_table()
{
	__dump_page_table(2, (vaddr_t)&page_table_root[0]);
}

__unused void page_table_test()
{
	show(&page_table_root);

	__unused paddr_t root = get_pa((vaddr_t)&page_table_root);
	show(root);	
}

__unused void test_linear_map()
{
	__unused vaddr_t va = get_emod_manager_pa_start() + linear_map_offset;
	__unused paddr_t pa = get_pa(va);

	show(va);
	show(pa);
}