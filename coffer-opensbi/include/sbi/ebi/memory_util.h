#pragma once
#include <types.h>
#include <memory/page_table.h>
#include <sbi/sbi_trap.h>

int copy_from_user(paddr_t maddr, vaddr_t uaddr, usize size);
int copy_from_user_not_aligned(paddr_t maddr, vaddr_t uaddr, usize size);
int copy_from_user_to_user(vaddr_t dst_uaddr, vaddr_t src_uaddr, usize size);
paddr_t get_pa(pte_t *table_root, vaddr_t va);
void* get_pt_root(void);