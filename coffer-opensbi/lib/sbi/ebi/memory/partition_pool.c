#include <enclave/eid.h>
#include <memory/page_table.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/ebi/enclave.h>
#include <sbi/ebi/ipi.h>
#include <sbi/ebi/partition_pool.h>
#include <sbi/ebi/pmp.h>
#include <sbi/riscv_locks.h>
#include <sbi/sbi_string.h>
#include <sbi/sbi_system.h>
#include <sbi/sbi_types.h>
#include <sbi/ebi/region.h>
#include <sbi/ebi/eval.h>
#include <sbi/sbi_list.h>
#include <sbi/riscv_barrier.h>

// typedef struct {
// 	paddr_t pa[NUM_REGIONS];
// 	usize size[NUM_REGIONS];
// 	int cnt;
// } region_t;

// static region_t mem_regions[NUM_ENCLAVE + 1];

static spinlock_t memory_pool_lock, compaction_lock;

#ifdef __QEMU__
static const paddr_t base = 0x240000000UL; // TODO tmp
#elif defined __UNMATCHED__
static const paddr_t base = 0x240000000UL; // TODO tmp
#else
#error "unknown arch"
#endif

static u64 enclave_partition_pool[NUM_PARTITIONS];
static usize avail_count = NUM_PARTITIONS;

static paddr_t emod_manager_start_pa_ptr_table[NUM_ENCLAVE + 1];
static paddr_t umode_pool_pa_aligned_ptr_table[NUM_ENCLAVE + 1];

usize get_pool_base()
{
    return base;
}

int is_in_pool(paddr_t pa)
{
    return (pa >= base && pa < base + POOL_SIZE);
}

void acquire_partition_pool(void)
{
    spin_lock(&memory_pool_lock);
}

void release_partition_pool(void)
{
    spin_unlock(&memory_pool_lock);
}

// #define acquire_partition_pool() do {   
//     sbi_DEBUG("lock pool\n");           
//     acquire_partition_pool();           
// } while (0)

// #define release_partition_pool() do {   
//     sbi_DEBUG("unlock pool\n");         
//     release_partition_pool();           
// } while (0)

static void update_partition_ownership(usize begin, usize end, u64 owner)
{
    usize i;
    for (i = begin; i < end; ++i)
        enclave_partition_pool[i] = owner;
}

void enclave_memory_clear(u64 eid)
{
    sbi_debug("clear memory of enclave %lu\n", eid);

    // usize free_count = 0;
    acquire_partition_pool();

    enclave_region_t *enc_reg = get_enclave_regions(eid);
	region_t *cur;

    sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
        if (!is_in_pool(cur->pa))
            continue;
        sbi_memset((void *)cur->pa, 0, cur->size);
        usize idx = PA_TO_IDX(cur->pa);
        update_partition_ownership(idx, idx + (cur->size >> PARTITION_SHIFT), -1);
        // free_count += cur->size;
        avail_count += cur->size >> PARTITION_SHIFT;
    }
    // avail_count += free_count;
    flush_tlb();
    release_partition_pool();

    sbi_debug("clear region data\n");
    enclave_free_regions(eid);
}

static usize alloc_n_partitions(usize n, usize *suggestion)
{
    usize i, max_idx = -1, ret_idx = -1;
    usize head = 0;
    u64 owner;
    bool scanning_free = false;
    usize max_len = 0, len;

    show(n);

    for (i = 0; i < NUM_PARTITIONS; i++) {
        owner = enclave_partition_pool[i];
        if (owner != -1) {
            if (scanning_free) {
                len = i - head;
                scanning_free = false;
                sbi_debug("len updated: len = %lu, head = %lu\n", len, head);

                if (max_len < len) {
                    sbi_debug("max len updated\n");
                    max_len = len;
                    max_idx = head;
                }
            }
        } else {
            if (!scanning_free) {
                head = i;
                scanning_free = true;
                sbi_debug("start to scan free, head = %lu\n", head);
            }
        }
    }
    if (scanning_free) {
        len = i - head;
        if (max_len < len) {
            max_len = len;
            max_idx = head;
        }
    }

    if (max_len >= n)
        ret_idx = max_idx;

    show(ret_idx);
    show(max_len);

    if (ret_idx != -1) {
        ret_idx += (max_len - n) >> 1;
    } else {
        if (!suggestion) {
            LOG(suggestion);
            panic("invalid pointer\n");
        }
        show(max_len);
        *suggestion = max_len;
        if (max_len == 0) {
            dump_partition_ownership();
            panic("Stall\n");
        }
    }

    show(ret_idx);

    return ret_idx;
}

// static void free_n_partitions(paddr_t pa, usize n)
// {
//     // sanity check

//     // boundary check

//     // update ownership

//     // update region

//     // clear memory
// }

static usize expand_region(u64 eid, region_t *reg, usize n)
{
    usize ret_idx = -1;
    usize bottom = PA_TO_IDX(reg->pa);
    usize top = bottom + (reg->size >> PARTITION_SHIFT);
    usize left = bottom;
    usize right = top;

    --left;
    while (left >= 0 && enclave_partition_pool[left] == -1) {
        --left;
    }
    ++left;

    while (right < NUM_PARTITIONS && enclave_partition_pool[right] == -1) {
        ++right;
    }

    if (bottom - left > right - top) {
        if (bottom - left >= n) {
            ret_idx = bottom - n;
        }
    } else {
        if (right - top >= n) {
            ret_idx = top;
        }
    }

    return ret_idx;
}

// static inline bool is_in_range(paddr_t pa, paddr_t src_pa, usize size)
// {
//     return (pa >= src_pa && pa < src_pa + size);
// }

// // return: page table addr
// static paddr_t update_satp(u64 eid, paddr_t src_pa, usize size, usize pa_diff)
// {
//     /** if the owner enclave is on-core,
//         its context will not be in the context array
//         since it is not suspended.
//         In this case, read from csr_satp directly.
//     */

//     u64 satp_value;
//     u64 on_core_eid = get_current_eid();
//     if (on_core_eid == eid)
//         satp_value = csr_read(CSR_SATP);
//     else
//         satp_value = get_enclave_satp(eid);
//     csr_satp_t satp = *(csr_satp_t *)&satp_value;

//     paddr_t pt_root_addr = satp.ppn << PAGE_SHIFT;
//     if (is_in_range(pt_root_addr, src_pa, size))
//         pt_root_addr += pa_diff;
//     else
//         return pt_root_addr;

//     satp.ppn = pt_root_addr >> PAGE_SHIFT;
//     satp_value = *(u64 *)&satp;

//     if (on_core_eid == eid)
//         csr_write(CSR_SATP, satp_value);
//     else
//         set_enclave_satp(eid, satp_value);

//     sbi_debug("new page table root addr: 0x%lx\n", pt_root_addr);
//     sbi_debug("satp updated: enclave %lu, new satp_value: 0x%lx\n",
//         eid, satp_value);

//     return pt_root_addr;
// }

// static void update_addr_record(u64 eid, paddr_t src_pa, usize size, usize pa_diff)
// {
//     paddr_t emod_manager_start_pa_ptr = emod_manager_start_pa_ptr_table[eid];
//     paddr_t umode_pool_pa_aligned_ptr = umode_pool_pa_aligned_ptr_table[eid];
//     volatile paddr_t emod_manager_start_pa = 
//         *(volatile paddr_t *)emod_manager_start_pa_ptr;
//     volatile paddr_t umode_pool_pa_aligned = 
//         *(volatile paddr_t *)umode_pool_pa_aligned_ptr;

//     if (is_in_range(emod_manager_start_pa_ptr, src_pa, size)) {
//         emod_manager_start_pa_ptr_table[eid] += pa_diff;
//         emod_manager_start_pa_ptr += pa_diff;
//     }
//     if (is_in_range(umode_pool_pa_aligned_ptr, src_pa, size)) {
//         umode_pool_pa_aligned_ptr_table[eid] += pa_diff;
//         umode_pool_pa_aligned_ptr += pa_diff;
//     }
//     if (is_in_range(emod_manager_start_pa, src_pa, size)) {
//         *(volatile u8 *)emod_manager_start_pa_ptr += pa_diff;
//     }
//     if (is_in_range(umode_pool_pa_aligned, src_pa, size)) {
//         *(volatile u8 *)umode_pool_pa_aligned_ptr += pa_diff;
//     }
// }

// static void update_pte(pte_t *pte, paddr_t src_pa, usize size, usize pa_diff)
// {
//     paddr_t pa = pte->ppn << PAGE_SHIFT;
//     if (is_in_range(pa, src_pa, size))
//         pte->ppn += (pa_diff >> PAGE_SHIFT);
// }

// static inline bool not_in_linear_map(vaddr_t va)
// {
// #define EMOD_MANAGER_LINEAR_START	0x140000000UL
// #define EMOD_MANAGER_LINEAR_SIZE	0x240000000UL
// #define EMOD_MANAGER_LINEAR_OFFSET	0xffffffc000000000UL
//     vaddr_t linear_start = EMOD_MANAGER_LINEAR_OFFSET + EMOD_MANAGER_LINEAR_START;
//     vaddr_t linear_end = linear_start + EMOD_MANAGER_LINEAR_SIZE;

//     if (va >= linear_start && va < linear_end)
//         return false;
//     else
//         return true;
// }

// static void update_page_table(
//     u8      level,
//     paddr_t table_pa,
//     paddr_t src_pa,
//     usize   size,
//     usize   pa_diff
// )
// {
// 	if (level > 2)
// 		panic("invalid level\n");

// #define NUM_ENTRY_PER_TABLE	(PAGE_SIZE / sizeof(pte_t))
// 	pte_t* root = (pte_t *)table_pa;

// 	static sv39_vaddr_t vaddr = {
// 		.extension = 0,
// 		.offset = 0,
// 		.vpn0 = 0,
// 		.vpn1 = 0,
// 		.vpn2 = 0,
// 	};

// 	for (int i = 0; i < NUM_ENTRY_PER_TABLE; i++) {
// 		pte_t *pte = &root[i];

// 		if (!pte->v)
// 			continue;

// 		if (level == SV39_LEVEL_PAGE)
// 			vaddr.vpn0 = i;
// 		else if (level == SV39_LEVEL_MEGA)
// 			vaddr.vpn1 = i;
// 		else {
// 			vaddr.vpn2 = i;
// 			vaddr.extension = (vaddr.vpn2 >> 8) ?
// 				(1UL << 25) - 1 :
// 				0;
// 		}

// 		if (pte->r | pte->w | pte->x) { // leaf pte
// 			vaddr_t va = sv39_to_va(vaddr);
//             if (likely(not_in_linear_map(va)))
//                 update_pte(pte, src_pa, size, pa_diff);
// 		} else {
//             update_pte(pte, src_pa, size, pa_diff);
// 			paddr_t next_pa = pte->ppn << PAGE_SHIFT;
// 			update_page_table(level + 1, next_pa, src_pa, size, pa_diff);
// 		}
// 	}

// 	if (level == SV39_LEVEL_PAGE)
// 		vaddr.vpn0 = 0;
// 	else if (level == SV39_LEVEL_MEGA)
// 		vaddr.vpn1 = 0;
// 	else {
// 		vaddr.vpn2 = 0;
// 		vaddr.extension = 0;
// 	}

//     flush_tlb();
// }
/** @brief migrate a memory region of an enclave
 * 
 * Caller should ensure all partitions to be migrated belong to the same enclave
 **/
// __unused static void region_migration(u64 eid, usize src_idx, usize dst_idx, usize n)
// {
//     /**
//      * 1. Copy and set owner
//      * 2. Update satp if necessary
//      * 3. Update page table
//      * 4. Clear source region
//      */

//     paddr_t src_pa = IDX_TO_PA(src_idx);
//     paddr_t dst_pa = IDX_TO_PA(dst_idx);
//     usize size = n << PARTITION_SHIFT;
//     usize pa_diff = dst_pa - src_pa;
//     show(src_pa); show(src_idx);
//     show(dst_pa); show(dst_idx);
//     show(pa_diff); 
//     show(n); show(size);

//     sbi_memcpy((void *)dst_pa, (void *)src_pa, size);
//     update_partition_ownership(dst_idx, dst_idx + n, eid);

//     paddr_t pt_root_addr = update_satp(eid, src_pa, size, pa_diff);
//     update_addr_record(eid, src_pa, size, pa_diff);
//     update_page_table(0, pt_root_addr, src_pa, size, pa_diff);

//     sbi_memset((void *)src_pa, 0, size);
//     update_partition_ownership(src_idx, src_idx + n, -1);
// }

// mem_region update
// static void partition_swap(usize x, usize y)
// {
//     usize tmp = -1;
//     usize i;
//     u64 x_owner = enclave_partition_pool[x];
//     u64 y_owner = enclave_partition_pool[y];

//     if (x_owner == -1) {
//         region_migration(y_owner, y, x, 1);
//     } else if (y_owner == -1) {
//         region_migration(x_owner, x, y, 1);
//     } else {
//         for (i = 0; i < NUM_PARTITIONS; i++) {
//             if (enclave_partition_pool[i] == -1) {
//                 tmp = i;
//                 break;
//             }
//         }
//         if (tmp == -1) {
//             panic("Error: No space for compaction!\n");
//         }

//         region_migration(x_owner, x, tmp, 1);
//         region_migration(y_owner, y, x, 1);
//         region_migration(x_owner, tmp, y, 1);
//         // tmp partition cleared afterwards
//     }

//     sbi_debug("%lu and %lu swapped\n", x, y);
// }

// static void memory_compaction(void)
// {
//     u64 owner_eid;
//     usize i, j;

//     dump_partition_ownership();
//     u64 eid = get_current_eid();
//     sbi_printf("eid == %lu\n", eid);
//     sbi_printf("[%s] STOP THE WORLD!\n", __func__);

//     // force the first partition to be used by some enclave
//     if (enclave_partition_pool[0] == -1) {
//         for (j = 1; j < NUM_PARTITIONS; j++) {
//             if (enclave_partition_pool[j] != -1) {
//                 break;
//             }
//         }
//         if (j != NUM_PARTITIONS) {
//             partition_swap(0, j);
//         }
//     }

//     // compact memory regions
//     for (i = 1; i < NUM_PARTITIONS; i++) {
//         if (enclave_partition_pool[i] == enclave_partition_pool[i - 1]) {
//             continue;
//         }

//         owner_eid = enclave_partition_pool[i - 1];
//         for (j = i + 1; j < NUM_PARTITIONS; j++) {
//             if (enclave_partition_pool[j] == owner_eid) {
//                 break;
//             }
//         }
//         if (j != NUM_PARTITIONS) {
//             partition_swap(i, j);
//         }
//     }

//     // rebuild region info
//     for (i = 0; i < NUM_PARTITIONS; i++) {
//         owner_eid = enclave_partition_pool[i];
//         sbi_debug("partition %lu: owner = %ld\n", i, (s64)owner_eid);

//         if (owner_eid == -1) {
//             continue;
//         }
//         for (j = i + 1; j < NUM_PARTITIONS; j++) {
//             if (enclave_partition_pool[j] != owner_eid) {
//                 break;
//             }
//         }

//         sbi_memset(&mem_regions[owner_eid], 0, sizeof(region_t));
//         mem_regions[owner_eid].pa[0] = IDX_TO_PA(i);
//         mem_regions[owner_eid].size[0] = (j - i) << PARTITION_SHIFT;
//         mem_regions[owner_eid].cnt = 1;

//         i = j;
//     }

//     sbi_debug("compaction done\n");

//     sbi_printf("[%s] RESUME THE WORLD!\n", __func__);
//     dump_partition_ownership();

//     dump_region();
// }

void wait_for_compaction(void)
{
    while (spin_lock_check(&compaction_lock))
        ;
    
    // todo: update permission here
    // u64 eid = get_current_eid();
    // region_update_permission(eid);
}

// int __try_region_migration(u64 eid)
// {
//     START_TIMER(mem_migration, eid);

//     region_info_t smallest_info = get_smallest_region(eid);
//     enclave_region_t *enc_reg = get_enclave_regions(eid);
//     region_t *cur;
//     usize res = -1UL;
//     int ret;

//     show(smallest_info.pa);
//     show(smallest_info.size);

//     sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
//         if (cur->pa == smallest_info.pa || !is_in_pool(cur->pa)) {
//             continue;
//         }

//         res = expand_region(eid, cur, smallest_info.size >> PARTITION_SHIFT);
//         if (res != -1UL) {
//             sbi_DEBUG("Migrate: Enclave %lu\n", eid);
//             show(smallest_info.pa);
//             show(smallest_info.size);
//             show(cur->pa);
//             show(cur->size);
//             show(IDX_TO_PA(res));
// // __dump_region_list(eid);

//             region_migration(eid, PA_TO_IDX(smallest_info.pa),
//             res, smallest_info.size >> PARTITION_SHIFT);

//             __enclave_add_region(eid, IDX_TO_PA(res), smallest_info.size);
//             __enclave_remove_region(eid, smallest_info.pa);
//             break;
//         }
//     }

//     ret = (res == -1UL) ? -1 : 0;

//     if (!ret) {
//         ADD_COUNTER(mem_migration, eid);
//         sbi_debug("migration successed\n");
// // __dump_region_list(eid);
//     }

//     STOP_TIMER(mem_migration, eid);

//     return ret;
// }

// int try_region_migration(u64 eid)
// {
//     int ret;
//     acquire_partition_pool();
//     ret = __try_region_migration(eid);
//     release_partition_pool();
//     return ret;
// }

// #define lock_region() do {			
// 	sbi_DEBUG("lock region\n");		
// 	lock_region();					
// } while (0)

// #define unlock_region() do {		
// 	sbi_DEBUG("unlock region\n");	
// 	unlock_region();				
// } while (0)

paddr_t alloc_partitions_for_enclave(
    u64 eid,
    usize number_of_partitions,
    usize *suggestion,
    int is_inst
)
{
    usize ret_idx;
    enclave_region_t *enc_reg = get_enclave_regions(eid);
    region_t *cur;

    if (number_of_partitions > avail_count) {
        dump_partition_ownership();
        LOG(number_of_partitions);
        LOG(avail_count);
        LOG(NUM_PARTITIONS);
        panic("Out of Memory\n");
    }

    show(number_of_partitions);
    show(avail_count);

    if (eid == HOST_EID) {
        panic("Error: Not supporting alloction for host OS\n");
    }

    lock_region();
    acquire_partition_pool();
    // Expand current region
    sbi_debug("expand current region\n");
    sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
        if (!is_in_pool(cur->pa))
            continue;

        ret_idx = expand_region(eid, cur, number_of_partitions);
        if (ret_idx != -1) {
            update_partition_ownership(ret_idx, ret_idx + number_of_partitions, eid);
            __enclave_add_region(eid, IDX_TO_PA(ret_idx), number_of_partitions << PARTITION_SHIFT, cur->is_inst);
            goto alloc_return;
        }
    }

    // New region (or new enclave)
    sbi_debug("allocate new region\n");
    ret_idx = alloc_n_partitions(number_of_partitions, suggestion);
    show(ret_idx);
    if (suggestion)
        show(*suggestion);
    if (ret_idx != -1) {
        update_partition_ownership(ret_idx, ret_idx + number_of_partitions, eid);
        __enclave_add_region(eid, IDX_TO_PA(ret_idx),
            number_of_partitions << PARTITION_SHIFT, is_inst);
        goto alloc_return;
    } else {
        goto fail_return;
    }

fail_return:
    flush_tlb();
    unlock_region();
    release_partition_pool();
    return -1UL;

alloc_return:
    // sbi_memset((void *)IDX_TO_PA(ret_idx),
        // 0, number_of_partitions << PARTITION_SHIFT);
    avail_count -= number_of_partitions;
    flush_tlb();
    unlock_region();
    release_partition_pool();
    return IDX_TO_PA(ret_idx);
}

void addr_record(
    paddr_t emod_manager_start_pa_ptr,
    paddr_t umode_pool_pa_aligned_ptr
)
{
    u64 eid = get_current_eid();
    emod_manager_start_pa_ptr_table[eid] =
        emod_manager_start_pa_ptr;
    umode_pool_pa_aligned_ptr_table[eid] =
        umode_pool_pa_aligned_ptr;
    return;
}

void clear_entire_pool()
{
    stop_other_harts();
    sbi_warn("Cleaning: base = 0x%lx, size = 0x%lx\n", base, POOL_SIZE);
    sbi_memset((void *)base, 0, POOL_SIZE);
    smp_mb();
    resume_other_harts();
}

int init_partition_pool(void)
{
    sbi_memset(enclave_partition_pool, -1, sizeof(enclave_partition_pool));
    SPIN_LOCK_INIT(&memory_pool_lock);
    SPIN_LOCK_INIT(&compaction_lock);

    region_init();
    return 0;
}

__unused void stop_other_harts()
{
    spin_lock(&compaction_lock);
    ipi_send_ebi_wait_for_compaction(-1);
}

__unused void resume_other_harts()
{
    spin_unlock(&compaction_lock);
}

__unused void dump_partition_ownership()
{
    int i;
    static const u32 entry_per_line = 6;
    sbi_printf("\n ##### dump partition ownership begin ### ");

    for (i = 0; i < NUM_PARTITIONS; i++) {
        if (i % entry_per_line == 0)
            sbi_printf("\n0x%lx:\t", IDX_TO_PA(i));
        sbi_printf("(%03d): ", i);

        u64 owner = enclave_partition_pool[i];
        if (owner == -1)
            sbi_printf("*");
        else
            sbi_printf("#%lu#", owner);

        sbi_printf("\t");
    }
    sbi_printf("\n");
    sbi_printf("##### dump partition ownership end   ### \n\n");
}
