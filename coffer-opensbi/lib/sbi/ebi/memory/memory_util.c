#include "memory/page_table.h"
#include "sys/cdefs.h"
#include <enclave/eid.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/ebi/memory_util.h>
#include <sbi/sbi_unpriv.h>
#include <util/gnu_attribute.h>
#include <sbi/sbi_system.h>

__unused static pte_t* get_pte(pte_t* root, uintptr_t va)
{
#define MASK_OFFSET 0xfff
#define MASK_L0 0x1ff000
#define MASK_L1 0x3fe00000
#define MASK_L2 0x7fc0000000
    uintptr_t layer_offset[] = { (va & MASK_L2) >> 30, (va & MASK_L1) >> 21,
        (va & MASK_L0) >> 12 };
    pte_t* tmp_entry;
    int i;
    for (i = 0; i < 3; ++i) {
        tmp_entry = &root[layer_offset[i]];
        if (!tmp_entry || !tmp_entry->v) {
            return NULL;
        }
        if ((tmp_entry->r | tmp_entry->w | tmp_entry->x)) {
            return tmp_entry;
        }
        root = (pte_t*)((uintptr_t)tmp_entry->ppn << PAGE_SHIFT);
    }
    return NULL;
}

// TODO: can be optimized
int copy_from_user_not_aligned(
    paddr_t maddr,
    vaddr_t uaddr,
    usize size
)
{
    __unused struct sbi_trap_info _;
    while (size > 0) {
        *(uint8_t*)maddr = sbi_load_u8((uint8_t*)uaddr, &_);
        ++maddr;
        ++uaddr;
        --size;

        if (_.cause) {
            sbi_error("Copy failed at 0x%lx, cause = 0x%lx\n", maddr, _.cause);
            panic("Stall\n");
            return -1;
        }
    }
    return 0;
}

int copy_from_user(
    paddr_t maddr,
    vaddr_t uaddr,
    usize size)
{
    __unused struct sbi_trap_info _;
    while (size > 0) {
        if (size >= 8) {
            *(uint64_t*)maddr = sbi_load_u64((uint64_t*)uaddr, &_);
            maddr += 8;
            uaddr += 8;
            size -= 8;
        } else if (size >= 4) {
            *(uint32_t*)maddr = sbi_load_u32((uint32_t*)uaddr, &_);
            maddr += 4;
            uaddr += 4;
            size -= 4;
        } else if (size >= 2) {
            *(uint16_t*)maddr = sbi_load_u16((uint16_t*)uaddr, &_);
            maddr += 2;
            uaddr += 2;
            size -= 2;
        } else {
            *(uint8_t*)maddr = sbi_load_u8((uint8_t*)uaddr, &_);
            ++maddr;
            ++uaddr;
            --size;
        }
        if (_.cause) {
            sbi_error("Copy failed maddr=0x%lx, uaddr=0x%lx, cause = 0x%lx\n",
                maddr, uaddr, _.cause);
            panic("Stall\n");
            return -1;
        }
    }
    return 0;
}

int copy_from_user_to_user(
    vaddr_t dst_uaddr,
    vaddr_t src_uaddr,
    usize size)
{
    __unused struct sbi_trap_info trap1;
    __unused struct sbi_trap_info trap2;
    while (size > 0) {
        if (size >= 8) {
            sbi_store_u64((uint64_t *)dst_uaddr,
                sbi_load_u64((uint64_t*)src_uaddr, &trap1),
                &trap2);
            dst_uaddr += 8;
            src_uaddr += 8;
            size -= 8;
        } else if (size >= 4) {
            sbi_store_u32((uint32_t *)dst_uaddr,
                sbi_load_u32((uint32_t*)src_uaddr, &trap1),
                &trap2);
            dst_uaddr += 4;
            src_uaddr += 4;
            size -= 4;
        } else if (size >= 2) {
            sbi_store_u16((uint16_t *)dst_uaddr,
                sbi_load_u16((uint16_t*)src_uaddr, &trap1),
                &trap2);
            dst_uaddr += 2;
            src_uaddr += 2;
            size -= 2;
        } else {
            sbi_store_u8((uint8_t *)dst_uaddr,
                sbi_load_u8((uint8_t*)src_uaddr, &trap1),
                &trap2);
            dst_uaddr++;
            src_uaddr++;
            size--;
        }
        if (trap1.cause) {
            sbi_error("Load failed src_uaddr=0x%lx, cause = 0x%lx\n",
                src_uaddr, trap1.cause);
            panic("Stall\n");
            __builtin_unreachable();
        }
        if (trap2.cause) {
            sbi_error("Store failed dst_uaddr=0x%lx, cause = 0x%lx\n",
                dst_uaddr, trap2.cause);
            panic("Stall\n");
            __builtin_unreachable();
        }
    }
    return 0;
}

void* get_pt_root(void)
{
    return (void*)((csr_read(satp) & 0xFFFFFFFFFFF) << 12);
}

struct walk_page_table_result {
	u8 		level;	// -1 for not valid
	pte_t	pte;
};

static struct walk_page_table_result walk_page_table(pte_t *table_root, vaddr_t va)
{
#define MASK_OFFSET 0xfff
#define MASK_L0 0x1ff000
#define MASK_L1 0x3fe00000
#define MASK_L2 0x7fc0000000
    u64 layer_offset[] = { (va & MASK_L2) >> 30, (va & MASK_L1) >> 21,
        (va & MASK_L0) >> 12 };
    pte_t* tmp_entry;
	pte_t* root = table_root;
	struct walk_page_table_result ret = {
		.level = -1,
	};
    int i;

    for (i = 0; i < 3; ++i) {
        tmp_entry = &root[layer_offset[i]];
        if (!tmp_entry->v) {
			sbi_printf("not valid\n");
            return ret;
        }
        if ((tmp_entry->r | tmp_entry->w | tmp_entry->x)) {
			ret.level = i;
			ret.pte = *tmp_entry;
            return ret;
        }

        root = (pte_t*)((u64)tmp_entry->ppn << PAGE_SHIFT);
    }
    return ret;
}

paddr_t get_pa(pte_t *table_root, vaddr_t va)
{
    show(table_root);
    show(va);

	struct walk_page_table_result result = walk_page_table(table_root, va);
	if (result.level == (u8)-1) {
		sbi_warn("VA 0x%lx not valid\n", va);
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