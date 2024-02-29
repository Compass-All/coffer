#include "sbi/sbi_types.h"
#include <enclave/eid.h>
#include <memory/memory.h>
#include <util/console.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/ebi/enclave.h>
#include <sbi/ebi/eval.h>
#include <sbi/ebi/partition_pool.h>
#include <sbi/ebi/pmp.h>
#include <sbi/riscv_asm.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_system.h>
#include <sbi/ebi/region.h>
#include <sbi/sbi_bitops.h>
#include <sbi/sbi_math.h>
#include <sbi/riscv_barrier.h>

/** Logical PMP
This C file aims to provide the following capabilities:
 1. generate logical PMP configurations according to
	enclave memory regions
 2. maintain enclave logical PMP contexts
 3. invoke low level PMP setters
*/

#define NUM_PMP	8

#define IDX_TO_ADDR_CSR(n) (CSR_PMPADDR0 + (n))

#if __riscv_xlen == 64
#define IDX_TO_CFG_CSR(n) ((CSR_PMPCFG0 + ((n) >> 2)) & ~1)
#define IDX_TO_CFG_SHIFT(n) (((n)&7) << 3)
#elif __riscv_xlen == 32
#define IDX_TO_CFG_CSR(n) (CSR_PMPCFG0 + ((n) >> 2))
#define IDX_TO_CFG_SHIFT(n) (((n)&3) << 3)
#else
#error "Unexpected __riscv_xlen"
#endif

#define PMP_A_OFF 0
#define PMP_ENABLE (PMP_R | PMP_W | PMP_X)
#define PMP_DISABLE 0

#define PMP_SET_IDX(idx, addr, perm, TYPE)                                 \
    do {                                                                   \
        int pmpaddr_csr = IDX_TO_ADDR_CSR(idx);                            \
        int pmpcfg_csr = IDX_TO_CFG_CSR(idx);                              \
        int pmpcfg_shift = IDX_TO_CFG_SHIFT(idx);                          \
        u64 cfg_mask = ~(0xFFUL << pmpcfg_shift);                          \
        u64 pmpcfg = csr_read_num(pmpcfg_csr) & cfg_mask;                  \
        pmpcfg |= ((perm | PMP_A_##TYPE) << pmpcfg_shift) & ~cfg_mask;     \
        csr_write_num(pmpaddr_csr, ((addr) >> PMP_SHIFT) & PMP_ADDR_MASK); \
        csr_write_num(pmpcfg_csr, pmpcfg);                                 \
    } while (0)

static bool is_power_of_two(u64 num)
{
	if (!num)
		return 0;

	return (num & (num - 1)) == 0;
}

__unused static int napot_power(paddr_t pa, usize size)
{
	if (is_power_of_two(size) && (pa % size == 0))
		return __ffs(size);
	return -1;
}

static void pmp_clear(void)
{
    for (int i = 0; i < NUM_PMP; ++i)
        PMP_SET_IDX(i, 0, PMP_DISABLE, OFF);
}

__unused void __pmp_dump(void)
{
	unsigned int i;
    int rc;
    ulong prot, addr, log2len;
    show(current_hartid());

	smp_mb();

	sbi_printf(KBLU "[Core %u] PMP configuration for enclave %lu:\n" RESET,
		current_hartid(), get_current_eid());
    for (i = 0; i < NUM_PMP; i++) {
        rc = pmp_get(i, &prot, &addr, &log2len);
        if (rc) {
            panic("PMP info read error!\n");
        }
		char *pmpmode = ((prot & PMP_A) == 0) 			? "Off" : 
						((prot & PMP_A) == PMP_A_NA4) 	? "NA4" :
						((prot & PMP_A) == PMP_A_NAPOT) ? "NAPOT" :
						((prot & PMP_A) == PMP_A_TOR) 	? "TOR" :
						"Error";
		char *perm_r = (prot & PMP_R) ? "R" : "-";
		char *perm_w = (prot & PMP_W) ? "W" : "-";
		char *perm_x = (prot & PMP_X) ? "X" : "-";
		sbi_printf("PMP %u: Mode: %s, Permission: %s%s%s, "
			"addr: 0x%lx, len: 0x%lx\n",
			i, pmpmode, perm_r, perm_w, perm_x,
			addr, 1UL << log2len);

        // sbi_printf("PMP#%u: prot=0x%lx, addr=0x%lx, log2len=%lu\n",
		// 	i, prot, addr, log2len);
    }

	smp_mb();
}

// #define lock_region() do {			
// 	sbi_DEBUG("lock region\n");		
// 	lock_region();					
// } while (0)

// #define unlock_region() do {		
// 	sbi_DEBUG("unlock region\n");	
// 	unlock_region();				
// } while (0)

__unused void pmp_dump(void)
{
	lock_region();
	__pmp_dump();
	unlock_region();
}

// return: number of consumed PMP index (1 or 2), 0 if out of index
static int set_pmp(int index, paddr_t pa, usize size)
{
    int ret = 0;

    // ----- early return for testing -----
    if (index < 0)   // --> 7 PMP
	// if (index < 1)   //--> 6 PMP
	// if (index < 2)   //--> 5 PMP
	// if (index < 3)   //--> 4 PMP
	// if (index < 4)   //--> 3 PMP
	// if (index < 5)   //--> 2 PMP
	// if (index < 6)   //--> 1 PMP
		return ret;
    // ------------------------------------

    int pow = napot_power(pa, size);
    if (pow != -1) {    // NAPOT
        if (index < 0)
            return ret;
        u64 napot_addr = pa | ((1UL << (pow - 1)) - 1);
        PMP_SET_IDX(index, napot_addr, PMP_ENABLE, NAPOT);
        ret = 1;
    } else {            // TOR
        if (index < 2)
            return ret;
		PMP_SET_IDX(index - 1, pa, PMP_ENABLE, OFF);
		PMP_SET_IDX(index, pa + size, PMP_ENABLE, TOR);
        ret = 2;
    }

    return ret;
}

static void __activate_host_pmp(enclave_region_t *host_reg, int pmp_count)
{
    int index = pmp_count - 1;
	PMP_SET_IDX(index, -1UL, PMP_DISABLE, NAPOT);
	index--;

    region_t *cur;
    sbi_list_for_each_entry(cur, &host_reg->reg_list, entry) {
		paddr_t pa = cur->pa;
		usize size = cur->size;

		int consumed = set_pmp(index, pa, size);
        if (!consumed)
            break;
        index -= consumed;
	}

    return;
}

static void __activate_enclave_pmp(enclave_region_t *enc_reg, int pmp_count)
{
    int index = pmp_count - 1;
	PMP_SET_IDX(index, -1UL, PMP_DISABLE, NAPOT);
	index--;

#define NUM_INST_REG 1
    usize inst_reg_count = 0;
    region_t *cur;
    sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
        if (!cur->is_inst)
            continue;
		paddr_t pa = cur->pa;
		usize size = cur->size;
        int consumed = set_pmp(index, pa, size);
        if (!consumed)
            break;
        index -= consumed;
        inst_reg_count++;
	}

    if (inst_reg_count < NUM_INST_REG) {
        LOG(inst_reg_count); LOG(NUM_INST_REG);
        panic("No enough instruction region to fill i-vPMP\n");
    }

    sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
        if (cur->is_inst)
            continue;
		paddr_t pa = cur->pa;
		usize size = cur->size;
        int consumed = set_pmp(index, pa, size);
        if (!consumed)
            break;
        index -= consumed;
	}

    return;
}

void activate_lpmp(u64 eid)
{
	lock_region();

	pmp_clear();

    smp_mb();
	enclave_region_t *enc_reg = get_enclave_regions(eid);
    if (eid == HOST_EID) {
        __activate_host_pmp(enc_reg, NUM_PMP);
    } else {
        __activate_enclave_pmp(enc_reg, NUM_PMP);
    }

	// debug
	// __pmp_dump();

	unlock_region();
}