#pragma once

#include "../rt_csr.h"
#include <util/memory.h>

// PTE fields
#define PTE_V 0x001 // Valid
#define PTE_R 0x002 // Read
#define PTE_W 0x004 // Write
#define PTE_X 0x008 // Execute
#define PTE_U 0x010 // User
#define PTE_G 0x020 // Global
#define PTE_A 0x040 // Accessed
#define PTE_D 0x080 // Dirty
#define PTE_SOFT 0x300 // Reserved for software

#define VA_PA_OFFSET() ((read_csr(satp) & (SATP_MODE_SV39 << SATP_MODE_SHIFT)) ? enc_va_pa_offset : 0)
#define VA_PA_OFFSET_NOMMU() (enc_va_pa_offset - VA_PA_OFFSET())

typedef uint16_t pte_attr_t;

typedef struct trie {
    uint32_t next[PAGE_DIR_POOL][1 << EPT_LEVEL_BITS], cnt;
} trie_t;

extern uintptr_t pt_root_pa;
extern size_t enc_va_pa_offset;

pte_t* get_pt_root(void);
trie_t* get_trie_root(void);
void map_page(uintptr_t va, uintptr_t pa, size_t n_pages, pte_attr_t attr, char do_insert_inverse_map);
uintptr_t alloc_page(uintptr_t va, uintptr_t n_pages, pte_attr_t attr, char idx);
uintptr_t usr_get_pa(uintptr_t va);
uintptr_t ioremap(pte_t* root, uintptr_t pa, size_t size);