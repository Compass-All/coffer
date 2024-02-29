#include <sbi/sbi_string.h>
#include <sbi/ebi/region.h>
#include <types.h>
#include <sbi/ebi/partition_pool.h>
#include <sbi/ebi/enclave.h>
#include <sbi/riscv_locks.h>
#include <sbi/ebi/ebi_debug.h>
#include <enclave/eid.h>
#include <sbi/sbi_system.h>
#include <sbi/sbi_list.h>
#include <sbi/riscv_barrier.h>
#include <sbi/ebi/pmp.h>

#define NUM_MAX_REGION	(NUM_PARTITIONS + 2 * NUM_ENCLAVE)

static region_t regions[NUM_MAX_REGION];
static enclave_region_t enclave_regions[NUM_ENCLAVE + 1];

static spinlock_t region_lock;

// __unused static void check_content()
// {
// 	hexdump((paddr_t)&enclave_regions, 512);
// }

static region_t *alloc_new_region()
{
	for (int i = 0; i < NUM_MAX_REGION; i++) {
		region_t *reg = &regions[i];
		if (!reg->size) {
			reg->pa = 0;
			SBI_INIT_LIST_HEAD(&reg->entry);
			return reg;
		}
	}

	panic("Out of region source\n");
	return NULL;
}

enclave_region_t *get_enclave_regions(u64 eid)
{
	return &enclave_regions[eid];
}

region_info_t get_smallest_region(u64 eid)
{
	region_info_t ret;
	usize min = -1UL;
	paddr_t pa = 0UL;

	enclave_region_t *enc_reg = get_enclave_regions(eid);
	region_t *cur;

	sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
		if (!is_in_pool(cur->pa)) // skip MMIO region
			continue;

		if (cur->size < min) {
			min = cur->size;
			pa = cur->pa;
		}
	}

	if (min == -1UL) {
		panic("Enclave owns no region\n");
	}

	ret.pa = pa;
	ret.size = min;

	return ret;
}

void lock_region()
{
	spin_lock(&region_lock);
}

void unlock_region()
{
	spin_unlock(&region_lock);
}

// #define lock_region() do {			
// 	sbi_DEBUG("lock region\n");		
// 	lock_region();					
// } while (0)

// #define unlock_region() do {		
// 	sbi_DEBUG("unlock region\n");
// 	unlock_region();				
// } while (0)

static inline void set_region(region_t *reg, paddr_t pa, usize size)
{
	reg->pa 	= pa;
	reg->size 	= size;
}

// TODO: implement this
static void __enclave_add_new_region(u64 eid, paddr_t pa, usize size,
    int is_inst)
{
	region_t *reg = alloc_new_region();
	enclave_region_t *enc_reg = get_enclave_regions(eid);
	reg->pa = pa;
	reg->size = size;
    reg->is_inst = is_inst;
	sbi_list_add_tail(&reg->entry, &enc_reg->reg_list);
	// sbi_list_add(&reg->entry, &enc_reg->reg_list);
	enc_reg->count++;
}

static int __enclave_expand_region(u64 eid, paddr_t pa, usize size)
{
	enclave_region_t *enc_reg = get_enclave_regions(eid);
	region_t *cur;

	sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
		paddr_t start = -1UL;
		if (pa + size == cur->pa) { // left adjacent
			start = pa;
		} else if (cur->pa + cur->size == pa) { // right adjacent
			start = cur->pa;
		} else if (cur->pa == pa && cur->size == size) {
			return 0; // MMIO device
		}

		if (start != -1UL) {
			sbi_debug("Add to original region\n");
			show(start);
			show(size + cur->size);
			set_region(cur, start, size + cur->size);
			return 0;
		}
	}

	return -1;
}

static int __enclave_hit_region(u64 eid, paddr_t pa)
{
	enclave_region_t *enc_reg = get_enclave_regions(eid);
	region_t *cur;

	show(eid);
	show(pa);

	sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
		if (cur->pa <= pa && pa < cur->pa + cur->size) {
			sbi_debug("Enclave %lu hit region 0x%lx, cnt = %lu\n",
				eid, cur->pa, cur->count);
			cur->count++;
			sbi_list_del_init(&cur->entry);
			sbi_list_add(&cur->entry, &enc_reg->reg_list);
			return 0;
		}
	}

	return -1;
}

static void __enclave_free_regions(u64 eid)
{
	enclave_region_t *enc_reg = get_enclave_regions(eid);
	region_t *cur;

	sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
		cur->count	= 0;
		cur->size	= 0;
		cur->pa		= 0;

		show(cur);
		show(cur->entry.prev);
		show(cur->entry.next);
	}

	SBI_INIT_LIST_HEAD(&enc_reg->reg_list);
	enc_reg->count = 0;
}

void enclave_free_regions(u64 eid)
{
	lock_region();
	__enclave_free_regions(eid);
	unlock_region();
}

void __enclave_add_region(u64 eid, paddr_t pa, usize size, int is_inst)
{
	int ret = __enclave_expand_region(eid, pa, size);
	if (ret)
		__enclave_add_new_region(eid, pa, size, is_inst);
}

void enclave_add_region(u64 eid, paddr_t pa, usize size, int is_inst)
{
	lock_region();
	__enclave_add_region(eid, pa, size, is_inst);
	unlock_region();
}

void __enclave_remove_region(u64 eid, paddr_t pa)
{
	enclave_region_t *enc_reg = get_enclave_regions(eid);
	region_t *cur;

	sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
		if (pa == cur->pa) {
			sbi_list_del_init(&cur->entry);
			enc_reg->count--;
			return;
		}
	}
}

int enclave_hit_region(u64 eid, paddr_t pa)
{
	int ret;
	sbi_debug("Got here\n");
	lock_region();
	sbi_debug("Got here 2\n");
	ret = __enclave_hit_region(eid, pa);
	// if (ret) {
	// 	unlock_region();
	// 	return ret;
	// }
	// if (eid != HOST_EID)
	// 	try_region_migration(eid);
	unlock_region();
	return ret;
}

void region_init()
{
	SPIN_LOCK_INIT(&region_lock);
	for (int i = 0; i < NUM_ENCLAVE + 1; i++) {
		enclave_region_t *enc_reg = get_enclave_regions(i);
		SBI_INIT_LIST_HEAD(&enc_reg->reg_list);
	}

	usize size_left = get_pool_base() - PHYS_MEM_START;
	usize size_right = PHYS_MEM_END - get_pool_base() - POOL_SIZE;

	enclave_add_region(HOST_EID, 0UL, PHYS_MEM_START, 0); // for testing purpose, disable IO protection

	enclave_add_region(HOST_EID, PHYS_MEM_START, size_left, 0);
	enclave_add_region(HOST_EID, get_pool_base() + POOL_SIZE, size_right, 0);
}

void region_reset()
{
	sbi_memset(regions, 0, sizeof(regions));
	sbi_memset(enclave_regions, 0, sizeof(enclave_regions));

	region_init();
}

__unused void __dump_region_list(u64 eid)
{
	region_t *cur;
	int i = 0;
	usize size = 0;
	enclave_region_t *enc_reg = get_enclave_regions(eid);
	sbi_printf("Dump region list of enclave %lu, count = %lu:\n",
		eid, enc_reg->count);
	
	if (!enc_reg->count) {
		return;
	}
	sbi_list_for_each_entry(cur, &enc_reg->reg_list, entry) {
		sbi_printf("\tRegion %d: "
            "pa = 0x%lx(%lu), size = 0x%lx(%lu), is_inst: %d\n",
			i,
			cur->pa, PA_TO_IDX(cur->pa),
			cur->size, cur->size >> PARTITION_SHIFT,
            cur->is_inst);

		i++;
		size += cur->size;
		if (i > enc_reg->count) {
			panic("Exceed max num");
		}
	}
	sbi_printf("Total size: 0x%lx\n", size);
	sbi_printf("------------------------------\n");
}

__unused void dump_region_list(u64 eid)
{
	lock_region();
	__dump_region_list(eid);
	unlock_region();
}

__unused void dump_region()
{
	for (usize i = 1; i <= NUM_ENCLAVE; i++) {
		u8 status = get_enclave_status(i);
		if (status)
			__dump_region_list(i);
	}
}