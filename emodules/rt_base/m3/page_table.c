#include "page_table.h"
#include "../rt_console.h"
#include "page_pool.h"
#include <stddef.h>

size_t enc_va_pa_offset;
uintptr_t pt_root_pa;
inverse_map_t inv_map[INVERSE_MAP_ENTRY_NUM];

#define rt_get_pa(va) (read_csr(satp) ? (va)-enc_va_pa_offset : (va))

pte_t* get_pt_root(void)
{
    return (pte_t*)(pt_root_pa + VA_PA_OFFSET());
}

trie_t* get_trie_root(void)
{
    return (trie_t*)((void*)get_pt_root() + EPAGE_SIZE * PAGE_DIR_POOL);
}

// First look up PA. If PA exists in the table, update it; otherwise
// insert a new entry.
// When updating, `count' must match with the previous one.
// Returns the newly inserted entry
static inverse_map_t* insert_inverse_map(uintptr_t pa, uintptr_t va,
    uint32_t count)
{
    int i = 0;

    em_debug("pa: 0x%lx, va: 0x%lx, count: %d\n", pa, va, count);
    for (; inv_map[i].pa && i < INVERSE_MAP_ENTRY_NUM; i++) {
        // if (pa == inv_map[i].pa) { // already exists; should update
        //     em_debug("updating entry; original va: 0x%lx, count: %d\n",
        //         va, count);
        //     if (count != inv_map[i].count) {
        //         em_error("Count does not match! original count: %d\n",
        //             inv_map[i].count);
        //         return NULL;
        //     }
        //     inv_map[i].va = va;
        // }
    }
    if (i == INVERSE_MAP_ENTRY_NUM) { // Out of entry
        em_error("NO ENOUGH ENTRY!!!\n");
        return NULL;
    }

    // New entry
    inv_map[i].pa = pa;
    inv_map[i].va = va;
    inv_map[i].count = count;

    return &inv_map[i];
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
    uint32_t p = 0, i = 0;
    page_directory* page_table = (page_directory*)get_pt_root();
    /*
     * [L2, L1, L0] PPN for each level, used fot trie to get offset of
     * page_directory_pool
     * Note: l[0] = L1, l[2] = l[0]
     */

    uintptr_t l[] = { (va & MASK_L2) >> 30, (va & MASK_L1) >> 21,
        (va & MASK_L0) >> 12 };
    pte_t* tmp_pte;

    // for a three level page table, only two PPNs need to point to next level
    for (; i < levels - 1; i++) {
        if (!root->next[p][l[i]]) {
            root->next[p][l[i]] = ++root->cnt;
            em_debug("\033[1;33mpage cnt:%d\033[0m\n", root->cnt);

            tmp_pte = &page_table[p][l[i]];
            tmp_pte->ppn = rt_get_pa((uintptr_t)&page_table[root->cnt][0]) >> 12;
            tmp_pte->pte_v = 1;
        }

        p = root->next[p][l[i]];
    }
    // Set items for the leaf page table entry
    tmp_pte = &page_table[p][l[levels - 1]];
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

    if (do_insert_inverse_map) {
        insert_inverse_map(pa, va, n_pages);
    }

    while (n_pages > 0) {
        page_insert(va, pa, 3, attr);
        va += EPAGE_SIZE;
        pa += EPAGE_SIZE;
        n_pages--;
    }

    if (read_csr(satp)) {
        flush_tlb();
    }
}

uintptr_t alloc_page(uintptr_t va, uintptr_t n_pages,
    pte_attr_t attr, char id)
{
    uintptr_t pa = 0, prev_pa = 0;
    inverse_map_t* inv_map_entry = NULL;

    em_debug("va = 0x%lx, n = %d\n", va, n_pages);
    while (n_pages >= 1) {
        pa = page_pool_get_pa(id);
        if (pa == prev_pa + EPAGE_SIZE && PARTITION_DOWN(pa) == PARTITION_DOWN(prev_pa)) {
            if (!inv_map_entry) {
                em_error("inv_map_entry is NULL!\n");
                return 0;
            }
            inv_map_entry->count++;
        } else {
            inv_map_entry = insert_inverse_map(pa, va, 1);
            if (!inv_map_entry) {
                em_error("inv_map_entry is NULL!\n");
                return 0;
            }
        }
        page_insert(va, pa, 3, attr);
        prev_pa = pa;
        va += EPAGE_SIZE;
        n_pages--;
    }

    if (read_csr(satp)) {
        flush_tlb();
    }
    return pa;
}

uintptr_t usr_get_pa(uintptr_t va)
{
    uintptr_t l[] = { (va & MASK_L2) >> 30, (va & MASK_L1) >> 21,
        (va & MASK_L0) >> 12 };
    pte_t* root = get_pt_root();
    pte_t tmp_entry;
    int i;
    for (i = 0; i < 3; ++i) {
        tmp_entry = root[l[i]];
        if (!tmp_entry.pte_v) {
            em_error("va:0x%lx is not valid!!!\n", va);
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
    drv_addr_alloc += n_pages << 12;
    return va;
}