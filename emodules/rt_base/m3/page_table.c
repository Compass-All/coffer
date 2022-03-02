#include "page_table.h"
#include "../rt_console.h"
#include "inv_map.h"
#include "page_pool.h"
#include <stddef.h>

size_t enc_va_pa_offset;
uintptr_t pt_root_pa;

#define VA_PA_OFFSET() (MMU_ENABLED() ? enc_va_pa_offset : 0)
#define get_pa(va) (read_csr(satp) ? usr_get_pa(va) : (va - enc_va_pa_offset));

pte_t* get_pt_root(void)
{
    return (pte_t*)(pt_root_pa + VA_PA_OFFSET());
}

trie_t* get_trie_root(void)
{
    return (trie_t*)((void*)get_pt_root() + EPAGE_SIZE * PAGE_DIR_POOL);
}

/**
 * Insert a va-pa pair to page table, maintained via a trie
 * @param va The virtual address
 * @param pa The physical address corresponding to va
 * @param levels The number of levels should be used. Note, it can be smaller than 3, which indicates a huge page
 * @param attr PTE attributes
 * @return
 */
static uintptr_t page_insert(uintptr_t va, uintptr_t pa, int levels,
    pte_attr_t attr)
{
    trie_t* root = get_trie_root();
    uint32_t p = 0, i;
    page_directory* page_table = (page_directory*)get_pt_root();
    /*
     * [L2, L1, L0] PPN for each level, used fot trie to get offset of
     * page_directory_pool
     * Note: vpn[0] = L1, vpn[2] = vpn[0]
     */

    uintptr_t vpn[] = { (va & MASK_L0) >> 12, (va & MASK_L1) >> 21, (va & MASK_L2) >> 30 };
    pte_t* tmp_pte;

    // for a three level page table, only two PPNs need to point to next level
    for (i = levels - 1; i > 0; --i) {
        if (!root->next[p][vpn[i]]) {
            root->next[p][vpn[i]] = ++root->cnt;
            em_debug("\033[1;33mpage cnt:%d\033[0m\n", root->cnt);

            tmp_pte = &page_table[p][vpn[i]];
            if (*(uintptr_t*)tmp_pte) {
                em_error("Invalid PTE 0x%llx\n", *(uintptr_t*)tmp_pte);
            }
            tmp_pte->ppn = rt_get_pa((uintptr_t)&page_table[root->cnt][0]) >> 12;
            tmp_pte->pte_v = 1;
        }

        p = root->next[p][vpn[i]];
    }
    // Set items for the leaf page table entry
    tmp_pte = &page_table[p][vpn[0]];
    tmp_pte->ppn = pa >> 12;
    if (levels == 2) {
        tmp_pte->ppn = (tmp_pte->ppn | MASK_OFFSET) ^ MASK_OFFSET;
    }
    tmp_pte->pte_v = tmp_pte->pte_g = 1;
    // tmp_pte->pte_v = 1;
    if (attr & PTE_U) {
        tmp_pte->pte_u = 1;
    }
    if (attr & PTE_W || attr & PTE_D) {
        tmp_pte->pte_d = tmp_pte->pte_w = 1;
    }
    if (attr & PTE_R || attr & PTE_A) {
        tmp_pte->pte_a = tmp_pte->pte_r = 1;
    }
    if (attr & PTE_X) {
        tmp_pte->pte_x = 1;
    }

    return *((uintptr_t*)tmp_pte);
}

void map_page(uintptr_t va, uintptr_t pa, size_t n_pages, pte_attr_t attr,
    char do_insert_inverse_map)
{
    if (n_pages == 0) {
        return;
    }

    em_debug("pa=0x%llx, va=0x%llx, count=%lu\n", pa, va, n_pages);
    while (n_pages > 0) {
        page_insert(va, pa, 3, attr);
        if (do_insert_inverse_map) {
            inv_map_insert(pa, va, 1);
        }
        va += EPAGE_SIZE;
        pa += EPAGE_SIZE;
        n_pages--;
    }

    if (MMU_ENABLED()) {
        flush_tlb();
    }
}

uintptr_t alloc_page(uintptr_t usr_va, uintptr_t n_pages,
    pte_attr_t attr, char idx)
{
    uintptr_t pa = -1, rt_va = -1;
    uintptr_t tmp_pa;
    size_t n_alloc, i;

    while (n_pages > 0) {
        n_alloc = page_pool_get(idx, &rt_va, n_pages);
        if (rt_va == -1) {
            em_error("Failed to allocate page from pool\n");
            return 0;
        }
        for (i = 0; i < n_alloc; ++i) {
            tmp_pa = get_pa(rt_va + i * EPAGE_SIZE);
            if (pa == -1) {
                pa = tmp_pa;
            }
            inv_map_insert(tmp_pa, usr_va, 1);
            page_insert(usr_va, tmp_pa, 3, attr);
            usr_va += EPAGE_SIZE;
        }
        n_pages -= n_alloc;
    }

    if (MMU_ENABLED()) {
        flush_tlb();
    }
    return pa;
}

uintptr_t usr_get_pa(uintptr_t va)
{
    uintptr_t vpn[] = { (va & MASK_L2) >> 30, (va & MASK_L1) >> 21,
        (va & MASK_L0) >> 12 };
    pte_t* root = get_pt_root();
    pte_t tmp_entry;
    int i;
    for (i = 0; i < 3; ++i) {
        tmp_entry = root[vpn[i]];
        if (!tmp_entry.pte_v) {
            em_error("va:0x%llx is not valid!!!\n", va);
            return 0;
        }
        if ((tmp_entry.pte_r | tmp_entry.pte_w | tmp_entry.pte_x)) {
            break;
        }
        root = (pte_t*)((tmp_entry.ppn << 12) + VA_PA_OFFSET());
    }
    if (i == 2)
        return (tmp_entry.ppn << 12) | (va & MASK_OFFSET);
    else if (i == 1)
        return ((tmp_entry.ppn >> 9) << 21) | (va & (MASK_L1 | MASK_OFFSET));
    else {
        return 0;
    }
}

uintptr_t ioremap(pte_t* root, uintptr_t pa, size_t size)
{
    static uintptr_t drv_addr_alloc = 0;
    uintptr_t va = ERT_DRV_START + drv_addr_alloc;
    em_debug("current root address: 0x%lx\n", get_pt_root());
    size_t n_pages = PAGE_UP(size) >> EPAGE_SHIFT;
    map_page(va, pa, n_pages, PTE_V | PTE_W | PTE_R | PTE_D | PTE_X, 0);
    drv_addr_alloc += n_pages << EPAGE_SHIFT;
    return va;
}