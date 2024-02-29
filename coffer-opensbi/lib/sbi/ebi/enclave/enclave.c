#include "enclave/threads.h"
#include "memory/riscv_barrier.h"
#include "message/short_message.h"
#include "sbi/sbi_bitmap.h"
#include "sbi/sbi_bitops.h"
#include "sbi/sbi_trap.h"
#include <assert.h>
#include <enclave/eid.h>
#include <memory/memory.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/ebi/enclave.h>
#include <sbi/ebi/iomngr.h>
#include <sbi/ebi/memory_util.h>
#include <sbi/ebi/partition_pool.h>
#include <sbi/ebi/pmp.h>
#include <sbi/ebi/eval.h>
#include <sbi/riscv_asm.h>
#include <sbi/riscv_atomic.h>
#include <sbi/riscv_encoding.h>
#include <sbi/riscv_locks.h>
#include <sbi/riscv_barrier.h>
#include <sbi/sbi_hartmask.h>
#include <sbi/sbi_list.h>
#include <sbi/sbi_string.h>
#include <sbi/sbi_system.h>
#include <sbi/sbi_unpriv.h>
#include <types.h>
#include <util/gnu_attribute.h>
#include <util/register.h>
#include <sbi/ebi/region.h>
#include <sbi/riscv_fp.h>
#include <sbi/sbi_types.h>

typedef struct {
	u64	eid;

#define ENCLAVE_FREE	0	// not initialized. no memory occupied
#define	ENCLAVE_INIT	1	// initialized. page table set up. ready to migrate
#define ENCLAVE_RUN		2	// running
#define ENCLAVE_IDLE	3	// suspended
	u8	status;

	u64 online_threads; 	// bitmap of on-core threads
	u64 alive_threads; 		// bitmap of alive threads
	u64 blocked_threads; 	// bitmap of blocked threads
	u64 thread_count;
	u64 thread_is_cloned;  // bitmap indicating whether threads are right after clone
							 // (both parent and child)
	u64 p_tid[NUM_THREADS];  // parent thread id 
	u64 num_fork;

    u64 clear_child_tid[NUM_THREADS]; // used by s mode

#define HARTID_OFFLINE	-1
	int	hartid;
	spinlock_t lock;
	// struct sbi_dlist entry;
} enclave_desc_t;

typedef struct {
	// m mode csr
	u64	mepc;
	u64	mstatus;
	u64 mie;
	u64 mip;
	u64 medeleg;
	u64 mideleg;

	// s mode csr
	u64 sepc;
	u64 sstatus;
	u64 satp;
	u64 stvec;
	u64 sie;
	u64 sip;
	u64 sscratch;

	// general purpose registers
	u64 regs[CTX_INDEX_MAX];

	// floating point registers
	u64 fprs[FPR_INDEX_MAX];
} enclave_context_t;

typedef struct {
	u64		eid;
	u64		tid;
	paddr_t smode_payload_pa_start;
	usize	smode_payload_size;
} boot_info_t;

typedef struct {
	paddr_t umode_payload_pa_start;
	usize	umode_payload_size;
} load_info_t;

static enclave_desc_t		enclave_desc[NUM_ENCLAVE + 1];	// enclave descriptor
static enclave_desc_t		host_desc[NUM_CORES + 1];		// enclave descriptor for host
static enclave_context_t	enclave_ctx[NUM_ENCLAVE + 1][NUM_THREADS];	// enclave context. 0th not used
static enclave_context_t	host_ctx[NUM_CORES];			// host context. one for each core
static u64					hartid_to_eid[NUM_CORES];		// a table of eid
static u64					hartid_to_tid[NUM_CORES];		// a table of tid
static spinlock_t 			eid_table_lock;
static spinlock_t 			tid_table_lock;
static u64 					eid_count = 1;
static spinlock_t 			eid_lock;

__unused void *memcpy(void *dest, const void *src, size_t count)
{
	return sbi_memcpy(dest, src, count);
}

__unused void *memset(void *s, int c, size_t count)
{
	return sbi_memset(s, c, count);
}

static inline void spin_lock_enclave(u64 eid)
{
    sbi_debug("Get enclave 0x%ld lock\n", eid);
    spin_lock(&enclave_desc[eid].lock);
}

static inline void spin_unlock_enclave(u64 eid)
{
    sbi_debug("Release enclave 0x%lx lock\n", eid);
    spin_unlock(&enclave_desc[eid].lock);
}

// invoked only once during boot
void init_enclave_desc()
{
	for (u64 i = 0; i <= NUM_ENCLAVE; i++) {
		enclave_desc[i] = (enclave_desc_t) {
			.eid 	= i,
			.status = ENCLAVE_FREE,
			.alive_threads = 0UL,
			.online_threads = 0UL,
            .blocked_threads = 0UL,
			.thread_count = 0UL,
            .thread_is_cloned = 0UL,	
			.hartid = HARTID_OFFLINE,
			.num_fork = 0
		};
        for (u64 j = 0; j < NUM_THREADS; j++) {
            enclave_desc[i].clear_child_tid[j] = 0;
			enclave_desc[i].p_tid[j] = -1UL;
        }
		SPIN_LOCK_INIT(&enclave_desc[i].lock);
	}

	for (u64 i = 0; i < NUM_CORES; i++) {
		host_desc[i] = (enclave_desc_t) {
			.eid 	= HOST_EID,
			.hartid = i
		};
		SPIN_LOCK_INIT(&host_desc[i].lock);
	}

	// ATOMIC_INIT(&num_initializing, 0);
	SPIN_LOCK_INIT(&eid_table_lock);
	SPIN_LOCK_INIT(&tid_table_lock);
}

static enclave_context_t *get_context_by_eid_tid(u64 eid, u64 tid)
{
	if (eid == HOST_EID) {
		// sbi_debug("host os context\n");
		u32 hart_id = current_hartid();
 		// show(hart_id);
		return &host_ctx[hart_id];
	} else {
		if (eid > NUM_ENCLAVE)
			panic("Invalid eid\n");
		// sbi_debug("enclave 0x%lx context\n", eid);
		// show(eid);
		return &enclave_ctx[eid][tid];
	}
}

static u64 get_eid_by_hartid(u32 hartid)
{
	u64 ret;
	spin_lock(&eid_table_lock);	
	ret = hartid_to_eid[hartid];
	spin_unlock(&eid_table_lock);
	return ret;
}

static void set_eid_by_hartid(u32 hartid, u64 eid)
{
	spin_lock(&eid_table_lock);
	hartid_to_eid[hartid] = eid;
	spin_unlock(&eid_table_lock);
}

static u64 get_tid_by_hartid(u32 hartid)
{
	u64 ret;
	spin_lock(&tid_table_lock);	
	ret = hartid_to_tid[hartid];
	spin_unlock(&tid_table_lock);
	return ret;
}

static void set_tid_by_hartid(u32 hartid, u64 tid)
{
	spin_lock(&tid_table_lock);
	hartid_to_tid[hartid] = tid;
	spin_unlock(&tid_table_lock);
}

u64 get_current_eid(void)
{
	u32 hartid = current_hartid();
	return get_eid_by_hartid(hartid);
}

u64 get_current_tid(void)
{
	u32 hartid = current_hartid();
	return get_tid_by_hartid(hartid);
}

u64 get_enclave_satp(u64 eid)
{
	enclave_context_t *context = get_context_by_eid_tid(eid, 0UL);	
	return context->satp;
}

// void set_enclave_satp(u64 eid, u64 tid, u64 satp_value)
// {
// 	enclave_context_t *context = get_context_by_eid_tid(eid, tid);	
// 	context->satp = satp_value;
// }

u64 get_current_mpp()
{
	u64 mstatus = csr_read(CSR_MSTATUS);
	return (mstatus & MSTATUS_MPP) >> MSTATUS_MPP_SHIFT;
}

static u8 __get_enclave_status(u64 eid)
{
    return enclave_desc[eid].status;
}

u8 get_enclave_status(u64 eid)
{
	u8 status;

	spin_lock_enclave(eid);
	status = __get_enclave_status(eid);
	spin_unlock_enclave(eid);

	return status;
}

// static void set_enclave_status(u64 eid, u8 status)
// {
// 	spin_lock_enclave(eid);
// 	__set_enclave_status(eid, status);
// 	spin_unlock_enclave(eid);	
// }

static inline u64 __get_online_threads(u64 eid)
{
    return enclave_desc[eid].online_threads;
}

static inline u64 __get_alive_threads(u64 eid)
{
    return enclave_desc[eid].alive_threads;
}

static u64 __get_blocked_threads(u64 eid)
{
    return enclave_desc[eid].blocked_threads;
}

u64 get_blocked_threads(u64 eid)
{
	spin_lock_enclave(eid);
    u64 ret = __get_blocked_threads(eid);
	spin_unlock_enclave(eid);
    show(ret);
    return ret;
}

void set_clear_child_tid(u64 eid, u64 tid, u64 tidptr)
{
    spin_lock_enclave(eid);
    enclave_desc[eid].clear_child_tid[tid] = tidptr;
    smp_mb();
	spin_unlock_enclave(eid);
}

u64 get_clear_child_tid(u64 eid, u64 tid)
{
	spin_lock_enclave(eid);
    smp_mb();
    u64 ret = enclave_desc[eid].clear_child_tid[tid];
	spin_unlock_enclave(eid);
    return ret;
}

// void wait_for_init_enclaves(void)
// {
//     u64 eid = get_current_eid();
//     long count = (enclave_desc[eid].status == ENCLAVE_INIT) ? 1 : 0;

//     while (atomic_read(&num_initializing) != count)
//         ;
// }

int dump_enclave_status()
{
	int active = 0;
	u32 alive_count = 0;
	for (u64 i = 1; i <= NUM_ENCLAVE; i++) {
		u8 status = get_enclave_status(i);
		if (status) {
			sbi_printf("### %lu: %s ", i, 
				status == ENCLAVE_INIT ? "INIT" :
				status == ENCLAVE_RUN  ? "RUNNING" :
				status == ENCLAVE_IDLE ? "IDLE" :
				"Unknown Status"
			);
			active = 1;
			if (status == ENCLAVE_IDLE)
				sbi_printf("sepc = 0x%lx", get_enclave_sepc(i, 0UL));
			sbi_printf("\n");
			if (status == ENCLAVE_IDLE || status == ENCLAVE_RUN) {
				alive_count++;
			}
		}
	}
	sbi_printf("Alive: %u\n", alive_count);
	return active;
}

u64 get_alive_count()
{
	u64 alive_count = 0;
	for (u64 i = 1; i <= NUM_ENCLAVE; i++) {
		u8 status = get_enclave_status(i);
		if (status == ENCLAVE_IDLE || status == ENCLAVE_RUN)
			alive_count++;
	}
	return alive_count;
}

static inline void __set_enclave_status(u64 eid, u8 status)
{
    enclave_desc[eid].status = status;
}


static u64 __new_alive_thread(u64 eid, u64 p_tid)
{
	u64 *alive_threads = &enclave_desc[eid].alive_threads;
	u64 new_tid = enclave_desc[eid].thread_count++;
	*alive_threads |= (1UL << new_tid);
	enclave_desc[eid].p_tid[new_tid] = p_tid;
	if (new_tid >= NUM_THREADS) {
		panic("Too many threads!\n");
	}
	sbi_debug("New thread of enclave %lu: thread %lu\n", eid, new_tid);
	return new_tid;
}

// return: tid of new thread
static u64 new_alive_thread(u64 eid, u64 p_tid)
{
	spin_lock_enclave(eid);
	u64 ret = __new_alive_thread(eid, p_tid);
	spin_unlock_enclave(eid);
	return ret;
}

static void __kill_thread(u64 eid, u64 tid)
{
    enclave_desc[eid].alive_threads &= ~(1UL << tid);
    enclave_desc[eid].online_threads &= ~(1UL << tid);
    enclave_desc[eid].blocked_threads &= ~(1UL << tid);
    enclave_desc[eid].thread_is_cloned &= ~(1UL << tid);
    sbi_memset(&enclave_desc[eid].clear_child_tid, 0,
        sizeof(enclave_desc[eid].clear_child_tid));
	sbi_debug("__kill_thread tid = %lu\n", tid);
	sbi_debug("__kill_thread p_tid[%lu] = %lu\n",
		tid, enclave_desc[eid].p_tid[tid]);
	// if child thread is going to be killed, unblock its parent.
	if (enclave_desc[eid].p_tid[tid] != -1UL) {
		enclave_desc[eid].blocked_threads &= ~(1UL << enclave_desc[eid].p_tid[tid]);
		enclave_desc[eid].p_tid[tid] = -1UL;
	}
	sbi_debug("blocked_threads=%lu\n", enclave_desc[eid].blocked_threads);
}

static void __mark_thread_cloned(u64 eid, u64 tid)
{
	show(eid); show(tid);
    u64 *thread_is_cloned = &enclave_desc[eid].thread_is_cloned;
    *thread_is_cloned |= (1UL << tid);
	show(*thread_is_cloned);
}

static void __mark_thread_not_cloned(u64 eid, u64 tid)
{
	show(eid); show(tid);
    u64 *thread_is_cloned = &enclave_desc[eid].thread_is_cloned;
    *thread_is_cloned &= ~(1UL << tid);
	show(*thread_is_cloned);
}

static int __thread_is_cloned(u64 eid, u64 tid)
{
	show(eid); show(tid);
    u64 thread_is_cloned = enclave_desc[eid].thread_is_cloned;
	show(thread_is_cloned);
    return (thread_is_cloned & (1UL << tid)) != 0;
}

inline static u64 thread_is_alive(u64 alive_threads, u64 tid)
{
	return alive_threads & (1UL << tid);
}

inline static u64 thread_is_online(u64 online_threads, u64 tid)
{
	return thread_is_alive(online_threads, tid); // reuse
}

inline static u64 thread_is_blocked(u64 blocked_threads, u64 tid)
{
	return thread_is_alive(blocked_threads, tid); // reuse
}

static void __thread_bring_online(u64 eid, u64 tid)
{
	u64 alive_threads = enclave_desc[eid].alive_threads;
	if (eid != HOST_EID && !thread_is_alive(alive_threads, tid)) {
		sbi_error("Enclave %lu thread %lu not alive (alive threads: 0x%lx)\n",
			eid, tid, alive_threads);
		panic("Try to bring up dead thread\n");
	}

	u64 *online_threads = &enclave_desc[eid].online_threads;
	if (eid != HOST_EID && thread_is_online(*online_threads, tid)) {
		sbi_error("Enclave %lu thread %lu already online (online threads: 0x%lx)\n",
			eid, tid, *online_threads);
		panic("Try to bring up an online thread\n");
	}

	*online_threads |= (1UL << tid);
	sbi_debug("Enclave %lu online threads: 0x%lx\n", eid, enclave_desc[eid].online_threads);
}

static void __thread_bring_offline(u64 eid, u64 tid)
{
	u64 alive_threads = enclave_desc[eid].alive_threads;
	if (eid != HOST_EID && !thread_is_alive(alive_threads, tid)) {
		sbi_error("Enclave %lu thread %lu not alive (alive threads: 0x%lx)\n",
			eid, tid, alive_threads);
		panic("Try to bring down dead thread\n");
	}

	u64 *online_threads = &enclave_desc[eid].online_threads;
	if (eid != HOST_EID && !thread_is_online(*online_threads, tid)) {
		sbi_error("Enclave %lu thread %lu already offline (online threads: 0x%lx)\n",
			eid, tid, *online_threads);
		panic("Try to bring down an offline thread\n");
	}

	*online_threads &= ~(1UL << tid);
	sbi_debug("Enclave %lu online threads: 0x%lx\n", eid, enclave_desc[eid].online_threads);
}

static void block_thread(u64 eid, u64 tid)
{
	spin_lock_enclave(eid);

    sbi_debug("Enclave %lu blocking thread 0x%lx\n", eid, tid);
    u64 *blocked_threads = &enclave_desc[eid].blocked_threads;
    *blocked_threads |= (1UL << tid);
	sbi_debug("Enclave %lu blocked threads: 0x%lx\n", eid, enclave_desc[eid].blocked_threads);

    u64 alive_threads = enclave_desc[eid].alive_threads;
    if (unlikely(alive_threads == *blocked_threads)) {
        sbi_error("All threads blocked!\n");
        panic("Dead Lock Detected\n");
    }

	spin_unlock_enclave(eid);
}

static void unblock_threads(u64 eid, u64 threads_to_unblock)
{
    spin_lock_enclave(eid);

    sbi_debug("Enclave %lu unblocking threads 0x%lx\n",
        eid, threads_to_unblock);
    u64 *blocked_threads = &enclave_desc[eid].blocked_threads;
    if (threads_to_unblock > *blocked_threads) {
        LOG(threads_to_unblock); LOG(*blocked_threads);
        panic("Trying to unblock unblocked threads\n");
    }
    *blocked_threads &= ~threads_to_unblock;
	sbi_debug("Enclave %lu blocked threads: 0x%lx\n", eid, enclave_desc[eid].blocked_threads);

	spin_unlock_enclave(eid);
}

// ---

void reset_coffer()
{
	spin_lock(&eid_lock);
	eid_count = 1;
	spin_unlock(&eid_lock);
}

u64 get_eid_count()
{
	u64 ret;
	spin_lock(&eid_lock);
	ret = eid_count;
	spin_unlock(&eid_lock);
	return ret;
}

// return 0 if not found
static u64 allocate_free_eid()
{
	// u64 free_eid = 0;

	// for (u64 i = 1; i <= NUM_ENCLAVE; i++) {
	// 	// note: do not use getter or setter here.
	// 	// since we need to get and set within the same critical section
	// 	spin_lock(&enclave_desc[i].lock);
	// 	if (enclave_desc[i].status == ENCLAVE_FREE) {
	// 		// sbi_debug("Free eid: 0x%lx\n", i);
	// 		free_eid = i;
	// 		enclave_desc[i].status = ENCLAVE_INIT;
	// 		spin_unlock(&enclave_desc[i].lock);
	// 		break;
	// 	}
	// 	spin_unlock(&enclave_desc[i].lock);
	// }

	// if (free_eid == 0)
	// 	sbi_error("No free enclave!\n");

	// return free_eid;

	// ----
	// monotonic increasing eid allocateor
	static int init = 0;
	u64 ret;

	if (!init) {
		SPIN_LOCK_INIT(&eid_lock);
		init = 1;
	}

	spin_lock(&eid_lock);
	ret = eid_count++;
	spin_unlock(&eid_lock);
	
	if (eid_count > NUM_ENCLAVE) {
		sbi_warn("Failed. Please reset eid\n");
		return HOST_EID;
	}

	return ret;
}

static enclave_context_t get_init_context(u64 mepc)
{
	u64 mstatus = MSTATUS_FS_CLEAN
		| (1UL << MSTATUS_MPP_SHIFT)	// mpp set to s mode
		| MSTATUS_SIE
		| MSTATUS_SUM;

	u64 mie = 0x2aaUL;
	u64 mip = 0UL;

	u64 medeleg = 0; // all traps in m mode

	// temporarily zero. need to be set for interrupt isolation
	u64 mideleg = 0x222UL;

	u64 sstatus = SSTATUS_FS;	// fs = dirty

	enclave_context_t init_context;

	init_context.mepc		= mepc;
	init_context.mstatus 	= mstatus;
	init_context.mie 		= mie;
	init_context.mip 		= mip;
	init_context.medeleg	= medeleg;
	init_context.mideleg	= mideleg;

	init_context.sepc		= 0UL;
	init_context.sstatus	= sstatus;
	init_context.satp		= 0UL;
	init_context.stvec		= 0UL;
	init_context.sie		= 0UL;
	init_context.sip		= 0UL;
	init_context.sscratch	= 0UL;

	sbi_memset(
		&init_context.regs,
		0,
		sizeof(init_context.regs)
	);

	sbi_memset(
		&init_context.fprs,
		0,
		sizeof(init_context.fprs)
	);

	return init_context;
}

static usize load_smode_payload(paddr_t dest_paddr)
{
	extern u8 _base_start, _base_end; // emod_manager.bin start and end pa
	paddr_t emod_manager_copy_start = (paddr_t)&_base_start,
		emod_manager_copy_end = (paddr_t)&_base_end;
	usize emod_manager_size =
		emod_manager_copy_end - emod_manager_copy_start;

	sbi_memcpy(
		(u8 *)dest_paddr,
		(u8 *)emod_manager_copy_start,
		emod_manager_size
	);

	return emod_manager_size;
}

static void save_umode_context(u64 eid, u64 tid, struct sbi_trap_regs *regs)
{
	// sbi_debug("Saving umode context\n");
	enclave_context_t *context = get_context_by_eid_tid(eid, tid);
	sbi_memcpy(
		context->regs,
		regs,
		EAPP_CTX_SIZE
	);
}

static void restore_umode_context(u64 eid, u64 tid, struct sbi_trap_regs *regs)
{
	// sbi_debug("Restoring umode context\n");
	enclave_context_t *context = get_context_by_eid_tid(eid, tid);
	sbi_memcpy(
		regs,
		context->regs,
		EAPP_CTX_SIZE
	);
}

/* Save float point registers
Float point contexts are not saved on the stack since
they are not used in M mode.
*/
static void save_float_context(u64 eid, u64 tid)
{
	enclave_context_t *context = get_context_by_eid_tid(eid, tid);
	if ((context->mstatus & MSTATUS_FS) == MSTATUS_FS_CLEAN)
		return; // clean state, early return
	else if (unlikely((context->mstatus & MSTATUS_FS) != MSTATUS_FS_DIRTY)) {
		sbi_error("Invalid mstatus FS field\n"); // not dirty nor clean, error
		panic("Stall\n");
	}

	// dirty state, save context
	for (int i = 0; i < FPR_INDEX_MAX; i++)
		context->fprs[i] = GET_F64_REG_DIRECT(i);
	context->mstatus &= (~MSTATUS_FS);
	context->mstatus |= MSTATUS_FS_CLEAN;
}

static void restore_float_context(u64 eid, u64 tid)
{
	enclave_context_t *context = get_context_by_eid_tid(eid, tid);
	if (unlikely((context->mstatus & MSTATUS_FS) != MSTATUS_FS_CLEAN)) {
		sbi_error("Invalid mstatus FS field\n"); // not clean, error
		panic("Stall\n");
	}
	for (int i = 0; i < FPR_INDEX_MAX; i++)
		SET_F64_REG_DIRECT(i, context->fprs[i]);
}

static void save_csr_context(u64 eid, u64 tid, struct sbi_trap_regs *regs)
{
	// sbi_debug("Saving csr context\n");
	enclave_context_t *context = get_context_by_eid_tid(eid, tid);

	context->mepc		= regs->mepc;
	context->mstatus	= regs->mstatus;
	context->mie		= csr_read(CSR_MIE);
	context->mip		= csr_read(CSR_MIP);
	context->medeleg	= csr_read(CSR_MEDELEG);
	context->mideleg	= csr_read(CSR_MIDELEG);

	context->sepc		= csr_read(CSR_SEPC);
	context->sstatus	= csr_read(CSR_SSTATUS);
	context->satp		= csr_read(CSR_SATP);
	context->stvec		= csr_read(CSR_STVEC);
	context->sie		= csr_read(CSR_SIE);
	context->sip		= csr_read(CSR_SIP);
	context->sscratch	= csr_read(CSR_SSCRATCH);
}

static void restore_csr_context(u64 eid, u64 tid, struct sbi_trap_regs *regs)
{
	// sbi_debug("Restoring csr context\n");
	enclave_context_t *context = get_context_by_eid_tid(eid, tid);

	regs->mepc		= context->mepc;
	regs->mstatus	= context->mstatus;
	csr_write(CSR_MIE, 		context->mie);
	csr_write(CSR_MIP, 		context->mip);
	csr_write(CSR_MEDELEG, 	context->medeleg);
	csr_write(CSR_MIDELEG, 	context->mideleg);

	csr_write(CSR_SEPC, 	context->sepc);
	csr_write(CSR_SSTATUS, 	context->sstatus);
	csr_write(CSR_SATP, 	context->satp);
	csr_write(CSR_STVEC, 	context->stvec);
	csr_write(CSR_SIE, 		context->sie);
	csr_write(CSR_SIP, 		context->sip);
	csr_write(CSR_SSCRATCH, context->sscratch);
}

// debug
__unused static void dump_enclave_context(u64 eid, u64 tid)
{
	__unused enclave_context_t *context = get_context_by_eid_tid(eid, tid);

	show(eid);

	sbi_debug("csr context:\n");
	show(context->mepc);
	show(context->mstatus);
	show(context->mie);
	show(context->mip);
	show(context->medeleg);
	show(context->mideleg);

	show(context->sepc);
	show(context->sstatus);
	show(context->satp);
	show(context->stvec);
	show(context->sie);
	show(context->sip);
	show(context->sscratch);

	sbi_debug("------- END\n");
}

// debug
__unused void DUMP_enclave_context(u64 eid, u64 tid)
{
	stop_other_harts();
	__unused enclave_context_t *context = get_context_by_eid_tid(eid, tid);

	sbi_DEBUG("csr context:\n");
	LOG(context->mepc);
	LOG(context->mstatus);
	LOG(context->mie);
	LOG(context->mip);
	LOG(context->medeleg);
	LOG(context->mideleg);

	LOG(context->sepc);
	LOG(context->sstatus);
	LOG(context->satp);
	LOG(context->stvec);
	LOG(context->sie);
	LOG(context->sip);
	LOG(context->sscratch);

	sbi_DEBUG("------- END\n");

	sbi_DEBUG("register dump:\n");
	for (int i = 1; i < CTX_INDEX_MAX; i++) {
		sbi_printf("x%d:\t0x%016lx\t", i, context->regs[i]);

		if (i % 3 == 0)
			sbi_printf("\n");
	}
	sbi_printf("\n");
	resume_other_harts();
}

static void save_context(u64 eid, u64 tid, struct sbi_trap_regs *regs)
{
	save_umode_context(eid, tid, regs);
	save_csr_context(eid, tid, regs);
	save_float_context(eid, tid);

	// dump_enclave_context(eid);
}

static void restore_context(u64 eid, u64 tid, struct sbi_trap_regs *regs)
{
	restore_umode_context(eid, tid, regs);
	restore_csr_context(eid, tid, regs);
	restore_float_context(eid, tid);

	// dump_enclave_context(eid);
}

#define dump_float_reg(reg_name) do {		\
	u64 reg_value;							\
	asm volatile("fmv.x.d %0, " #reg_name "\n\t" : "=r"(reg_value));	\
	sbi_debug(#reg_name": 0x%lx\n", reg_value);	\
} while (0)

__unused static void dump_all_float()
{
	sbi_info("start dump ----------\n");
	dump_float_reg(fa0);
	dump_float_reg(fa1);
	dump_float_reg(fa2);
	dump_float_reg(fa3);
	dump_float_reg(fa4);
	dump_float_reg(fa5);
	sbi_info("end dump ----------\n");
}

static void __enclave_switch(
	u64 last_eid,
	u64 last_tid,
	u64 next_eid,
	u64 next_tid,
	struct sbi_trap_regs *regs
)
{
	sbi_info("From enclave %lu thread %lu to enclave %lu thread %lu\n", last_eid, last_tid, next_eid, next_tid);

	u32 hart_id = current_hartid();
	
	save_context(last_eid, last_tid, regs);
	restore_context(next_eid, next_tid, regs);
	
	STOP_TIMER(execution, last_eid);
	START_TIMER(execution, next_eid);

	set_eid_by_hartid(hart_id, next_eid);
	set_tid_by_hartid(hart_id, next_tid);

	__thread_bring_offline(last_eid, last_tid);
	__thread_bring_online(next_eid, next_tid);

	dump_all_float();

	activate_lpmp(next_eid);
	flush_tlb();
	sbi_debug("switch into enclave %lu thread %lu\n", next_eid, next_tid);
	sbi_debug("regs->mepc = 0x%lx\n", regs->mepc);
}

/** allocate eid, allocate primary partition
 * load s mode payload (emod_manager),
 * set initial context
*/
static boot_info_t create_enclave()
{
	u64 eid = allocate_free_eid();
	paddr_t enclave_pa_start;
	usize	smode_payload_size;
	enclave_context_t *context;

	if (eid == HOST_EID) {
		return (boot_info_t) {
			.eid = HOST_EID,
			.smode_payload_pa_start = 0UL,
			.smode_payload_size = 0UL
		};
	}

	show(eid);

	init_timer(eid);

	static u8 first_enclave = 1;
	if (first_enclave) {
		clear_entire_pool();
		first_enclave = 0;
		flush_tlb();
	}

	START_TIMER(creation, eid);

	__set_enclave_status(eid, ENCLAVE_INIT);
	u64 tid = new_alive_thread(eid, -1UL);
	ASSERT(tid == 0UL, "new tid expected to be 0");

	// should this function be protected by lock?
	enclave_pa_start = alloc_partitions_for_enclave(eid, 1, NULL, 1);

	smode_payload_size = load_smode_payload(enclave_pa_start);

	context = get_context_by_eid_tid(eid, tid);
	*context = get_init_context(enclave_pa_start);

	return (boot_info_t) {
		.eid 					= eid,
		.tid					= tid,
		.smode_payload_pa_start	= enclave_pa_start,
		.smode_payload_size		= smode_payload_size
	};
}

/** allocate secondary partition,
 * load u mode payload (ELF),
 * copy argv
 */
static load_info_t enter_enclave(
	u64 	eid,
	u64		argc,
	vaddr_t argv,
	vaddr_t umode_payload_host_va,
	usize	umode_payload_size
)
{
	paddr_t umode_payload_pa_start;
	paddr_t umode_arg_addr;
	struct sbi_trap_info dummy_trap;

	usize number_of_partitions = 
		PARTITION_UP(umode_payload_size) / PARTITION_SIZE;
	show(number_of_partitions);

	umode_payload_pa_start =
		alloc_partitions_for_enclave(eid, number_of_partitions, NULL, 1);
	show(umode_payload_pa_start);

	copy_from_user(
		umode_payload_pa_start,
		umode_payload_host_va,
		umode_payload_size
	);

	umode_arg_addr = PAGE_UP(
		umode_payload_pa_start + umode_payload_size
	);
	// show(umode_arg_addr);

	u64 *host_argv_pointer = (u64 *)argv;
	u8	*enclave_arg_ptr = (u8 *)(umode_arg_addr + sizeof(u64) * argc);
	for (int i = 0; i < argc; i++) {
		// show(i);

		vaddr_t argv_i = sbi_load_u64(host_argv_pointer, &dummy_trap);
		// show(argv_i);

		*(u64 *)(umode_arg_addr + sizeof(u64) * i) =
			(u64)enclave_arg_ptr;

		u8 *ptr = (u8 *)argv_i;
		u8 value;
		do {
			value = sbi_load_u8(ptr++, &dummy_trap);
			*enclave_arg_ptr++ = value;
		} while (value);

		host_argv_pointer++;
	}

	__set_enclave_status(eid, ENCLAVE_RUN);

	return (load_info_t) {
		.umode_payload_pa_start = umode_payload_pa_start,
		.umode_payload_size		= umode_payload_size
	};
}

static void __exit_enclave(u64 eid)
{
	// debug
	// dump_region_list(eid);

	// free and clear enclave partitions
	enclave_memory_clear(eid);
	clear_device(eid);
	
	enclave_desc[eid].alive_threads = 0UL;
	enclave_desc[eid].online_threads= 0UL;
	enclave_desc[eid].blocked_threads = 0UL;
	enclave_desc[eid].thread_count= 0UL;
	enclave_desc[eid].thread_is_cloned = 0UL;

	__set_enclave_status(eid, ENCLAVE_FREE);
}

u64 get_enclave_sepc(u64 eid, u64 tid)
{
	enclave_context_t *context = get_context_by_eid_tid(eid, tid);
	return context->sepc;
}

int ebi_create_handler(struct sbi_trap_regs* regs)
{
	u64 current_eid = get_current_eid();

	if (current_eid != HOST_EID)
		panic("Error: enclaves must be created from host!\n");

	boot_info_t boot_info = create_enclave();
	show(boot_info.eid);

	sbi_debug("Enclave%lu created\n", boot_info.eid);

	__enclave_switch(HOST_EID, 0UL, boot_info.eid,
		boot_info.tid, regs);

	regs->a0 = boot_info.eid;
	regs->a1 = boot_info.smode_payload_pa_start;
	regs->a2 = boot_info.smode_payload_size;

	return 0;
}

/**
 * a0: eid
 * a1: argc
 * a2: argv
 * a3: umode_payload_host_va
 * a4: umode_payload_size
 */
int ebi_enter_handler(struct sbi_trap_regs* regs)
{
	u64		eid 	= regs->a0;
	u64		argc 	= regs->a1;
	vaddr_t	argv	= regs->a2;
	vaddr_t umode_payload_host_va 	= regs->a3;
	usize	umode_payload_host_size	= regs->a4;

	START_TIMER(total, eid);

	u64 current_eid = get_current_eid();

	if (current_eid != HOST_EID) {
		sbi_error("Error: enclaves must be entered from host!\n");
		return -1;
	}
	show(current_hartid());
	sbi_debug("Entering enclave %lu\n", eid);

	u8 status;
	if ( (status = get_enclave_status(eid)) != ENCLAVE_IDLE ) {
		show(status);
		sbi_error("Error: Enclave not in idle state!\n");
		return -1;
	}

	load_info_t load_info = enter_enclave(
		eid,
		argc,
		argv,
		umode_payload_host_va,
		umode_payload_host_size
	);

	__enclave_switch(HOST_EID, 0UL,
		eid, 0UL, regs);

	regs->a0 = load_info.umode_payload_pa_start;
	regs->a1 = load_info.umode_payload_size;
	regs->a2 = argc;

	return 0;
}

int ebi_suspend_handler(struct sbi_trap_regs* regs)
{
	u64 short_message = regs->a0;

	u64 current_eid = get_current_eid();
	u64 current_tid = get_current_tid();

    spin_lock_enclave(current_eid);

    u64 online_threads = enclave_desc[current_eid].online_threads;

	if (current_eid == HOST_EID)
		panic("Host cannot be suspended!\n");

	u8 status = __get_enclave_status(current_eid);
	if (status != ENCLAVE_RUN && status != ENCLAVE_INIT) {
        LOG(status); LOG(online_threads);
		panic("Enclave not running!\n");
    }
    if ((online_threads & (1 << current_tid)) == 0UL) {
        LOG(online_threads);
        panic("Thread not online\n");
    }
    if ((online_threads & ~(1 << current_tid)) == 0UL) {
        __set_enclave_status(current_eid, ENCLAVE_IDLE);
    }
	__enclave_switch(current_eid, current_tid,
		HOST_EID, 0UL, regs);

	__unused enclave_context_t *ctx = get_context_by_eid_tid(current_eid, current_tid);
	sbi_debug("Suspending enclave %lu, sepc = 0x%lx\n",
		current_eid, ctx->sepc);
	sbi_debug("Resuming to host: 0x%lx\n", regs->mepc);

	if (status == ENCLAVE_INIT) {
		STOP_TIMER(creation, current_eid);
		regs->a0 = current_eid;
	} else {
		regs->a0 = short_message;
	}
	show(regs->a0);

    spin_unlock_enclave(current_eid);

	return 0;
}

static int __sys_vfork_handler(struct sbi_trap_regs* regs)
{
	sbi_debug("vfork syscall handling\n");

    u64 eid = get_current_eid();
	u64 p_tid = get_current_tid();

	u64 c_tid = __new_alive_thread(eid, p_tid);  // already set alive_threads
	sbi_debug("Enclave %lu thread %lu cloning new thread %lu\n",
		eid, p_tid, c_tid);

	__mark_thread_cloned(eid, p_tid);
	__mark_thread_cloned(eid, c_tid);

	// copy stack
	show(regs->sp);
	vaddr_t p_stack_top = ROUNDUP(regs->sp, UMODE_STACK_SIZE);
	if (enclave_desc[eid].num_fork >= MAX_FORK) {
		sbi_error("Enclave %lu num_fork:%lu >= MAX_FORK.\n",
			eid, enclave_desc[eid].num_fork);
		panic("number of fork exceeded limit\n");
	}
	enclave_desc[eid].num_fork++;
	vaddr_t c_stack_top = UMODE_STACK_TOP_VA - 
		enclave_desc[eid].num_fork * UMODE_STACK_SIZE;
	show(p_stack_top); show(c_stack_top);
	copy_from_user_to_user(
		c_stack_top - UMODE_STACK_SIZE,
		p_stack_top - UMODE_STACK_SIZE,
		UMODE_STACK_SIZE
	);

    u64 alive_threads = enclave_desc[eid].alive_threads;
	
	if ((alive_threads & ~(1 << p_tid)) == 0)
        __set_enclave_status(eid, ENCLAVE_IDLE);
    __enclave_switch(eid, p_tid,
        HOST_EID, 0UL, regs);

	// fix: should use trap_regs context rather than stored context
	enclave_context_t *p_context = get_context_by_eid_tid(eid, p_tid);
	enclave_context_t *c_context = get_context_by_eid_tid(eid, c_tid);
	sbi_memcpy(c_context, p_context, sizeof(enclave_context_t));

	
	p_context->regs[CTX_INDEX_a0] = c_tid;
    p_context->mepc += 4UL;

	c_context->regs[CTX_INDEX_a0] = 0UL;
    c_context->mepc += 4UL;
	c_context->regs[CTX_INDEX_sp] -= UMODE_STACK_SIZE;

    show(p_context->mepc);
    show(c_context->mepc);

	// what does it do here?
    regs->a0 = NEW_THREAD | (u32)c_tid;
    regs->mepc -= 4;

	// block the parent thread after vfork a child.
	u64 *blocked_threads = &(enclave_desc[eid].blocked_threads);
    *blocked_threads |= (1UL << p_tid);
	
	alive_threads = enclave_desc[eid].alive_threads;
    if (unlikely(alive_threads == *blocked_threads)) {
        sbi_error("All threads blocked!\n");
        panic("Dead Lock Detected\n");
    }
	return 0;
}

/*
long clone(unsigned long flags, void *stack,
                     int *parent_tid, unsigned long tls,
					 int *child_tid);
*/
#define DEFAULT_STACK_SIZE 131072
int sys_clone_handler(struct sbi_trap_regs* regs)
{
	sbi_info("Clone syscall handling\n");

    u64 eid = get_current_eid();
	u64 p_tid = get_current_tid();

	spin_lock_enclave(eid);

	u64 flags = regs->a0;
	u64 c_stack_va = regs->a1;
	u64 parent_tid_ptr = regs->a2;
	u64 c_tls = regs->a3;
	u64 child_tid_ptr = regs->a4;

	int ret = 0;
	if (flags == 0x11UL) {
		ret = __sys_vfork_handler(regs);
		goto out;
	}
	
	if (flags != 0x7d0f00UL) {
		sbi_warn("Unusual clone flags 0x%lx, expected 0x%lx\n",
		flags, 0x7d0f00UL);
        panic("Stall\n");
    }
	
	show(flags);
	LOG(c_stack_va);
	show(parent_tid_ptr);
	show(child_tid_ptr);
	show(c_tls);
    show(regs->tp);

	if (flags == 0x11UL) {
		sbi_printf("sys_vfork_handler() is called\n");
		return __sys_vfork_handler(regs);
	}

	__unused u64 mpp = get_current_mpp();
	show(mpp);
	show(csr_read(CSR_MEPC));
	show(csr_read(CSR_SEPC));

	u64 c_tid = __new_alive_thread(eid, -1UL);
	sbi_debug("Enclave %lu thread %lu cloning new thread %lu\n",
		eid, p_tid, c_tid);
	__mark_thread_cloned(eid, p_tid);
	__mark_thread_cloned(eid, c_tid);

    // sbi_store_u32 uses current SATP so it must go before __enclave_switch
    struct sbi_trap_info trap;
	sbi_store_u32((u32 *) parent_tid_ptr, (u32)c_tid, &trap);
	if (trap.cause)
		sbi_error("Store failed at 0x%lx, cause = 0x%lx\n", parent_tid_ptr, trap.cause);
	sbi_store_u32((u32 *) child_tid_ptr, (u32)c_tid, &trap);
	if (trap.cause)
		sbi_error("Store failed at 0x%lx, cause = 0x%lx\n", child_tid_ptr, trap.cause);

    u64 alive_threads = enclave_desc[eid].alive_threads;
    enclave_desc[eid].clear_child_tid[c_tid] = child_tid_ptr;
    if ((alive_threads & ~(1 << p_tid)) == 0)
        __set_enclave_status(eid, ENCLAVE_IDLE);
    __enclave_switch(eid, p_tid,
        HOST_EID, 0UL, regs);

	// fix: should use trap_regs context rather than stored context
	enclave_context_t *p_context = get_context_by_eid_tid(eid, p_tid);
	enclave_context_t *c_context = get_context_by_eid_tid(eid, c_tid);
	sbi_memcpy(c_context, p_context, sizeof(enclave_context_t));

	p_context->regs[CTX_INDEX_a0] = c_tid;
    p_context->mepc += 4UL;

	c_context->regs[CTX_INDEX_a0] = 0UL;
	c_context->regs[CTX_INDEX_sp] = c_stack_va;
    c_context->regs[CTX_INDEX_tp] = c_tls;
    c_context->sscratch = 0UL;
    c_context->mepc += 4UL;

    show(p_context->mepc);
    show(c_context->mepc);

    regs->a0 = NEW_THREAD | (u32)c_tid;
    regs->mepc -= 4;

out:
	spin_unlock_enclave(eid);
	return ret;
}

int ebi_block_thread_handler(struct sbi_trap_regs *regs)
{
    u64 tid_to_block = regs->a0;
    u64 eid = get_current_eid();

    block_thread(eid, tid_to_block);

    return 0;
}

int ebi_unblock_threads_handler(struct sbi_trap_regs *regs)
{
    u64 threads_to_unblock = regs->a0;
    u64 eid = get_current_eid();

    unblock_threads(eid, threads_to_unblock);

    return 0;
}

int ebi_resume_handler(struct sbi_trap_regs* regs)
{
	u64 eid 			= regs->a0;
	u64 tid 			= regs->a1;
	u64 short_message	= regs->a2;

    spin_lock_enclave(eid);

	u64 current_eid = get_current_eid();

	if (current_eid != HOST_EID)
		panic("Error: enclave can only be resumed from host!\n");

    u64 alive_threads = __get_alive_threads(eid);
    u64 online_threads = __get_online_threads(eid);
    u64 blocked_threads = __get_blocked_threads(eid);
    show(alive_threads); show(online_threads); show(blocked_threads);

    if (thread_is_blocked(blocked_threads, tid)) {
        sbi_info("Thread is blocked, resume ignored\n");
        regs->a0 = BLOCKED;
        goto out;
    }

    if (unlikely(!thread_is_alive(alive_threads, tid))) {
        LOG(eid); LOG(tid); LOG(alive_threads);
        panic("thread not alive\n");
    }
    if (unlikely(thread_is_online(online_threads, tid))) {
        LOG(eid); LOG(tid); LOG(online_threads);
        panic("thread already online\n");
    }

    __set_enclave_status(eid, ENCLAVE_RUN);
	__enclave_switch(HOST_EID, 0UL,
		eid, tid, regs);  // check here
	sbi_debug("Resuming enclave %lu thread %lu\n", eid, tid);
    show(get_current_mpp())

    if (!__thread_is_cloned(eid, tid)) {
		sbi_debug("thread is not right after cloning!\n");
		regs->a0 = short_message;
	    show(short_message);
    } else {
        sbi_debug("This thread is right after cloning\n");
        show(regs->a0);
        __mark_thread_not_cloned(eid, tid);
    }

out:
    spin_unlock_enclave(eid);
	return 0;
}

int ebi_exit_thread_handler(struct sbi_trap_regs* regs)
{
    u64 ret = regs->a0;

	u64 current_eid = get_current_eid();
	u64 current_tid = get_current_tid();

	spin_lock_enclave(current_eid);
	sbi_debug("exit_thread_handler start\n");
	sbi_debug("ret = 0x%lu\n", ret);
	sbi_debug("eid = %lu, tid = %lu\n", current_eid, current_tid);
    
    show(current_hartid());

    if (current_eid == HOST_EID)
		panic("Error: enclave cannot be exited from host!\n");

    __enclave_switch(current_eid, current_tid, HOST_EID, 0UL, regs);
    __kill_thread(current_eid, current_tid);

	regs->a0 = ret;	// host context
	sbi_debug("exit_thread_handler finished\n");
    spin_unlock_enclave(current_eid);

    return 0;
}

int ebi_exit_handler(struct sbi_trap_regs* regs)
{
	u64 ret = regs->a0; // enclave return value
	
	u64 current_eid = get_current_eid();
	u64 current_tid = get_current_tid();
	
	spin_lock_enclave(current_eid);

    // dump_partition_ownership();
    // dump_region_list(current_eid);

	sbi_debug("ebi_exit_handler(enclave) start\n");	
	sbi_debug("eid = %lu, eid = %lu\n", current_eid, current_tid);
	show(current_hartid());
	if (current_tid == 0) {
		START_TIMER(clean_up, current_eid);

		if (current_eid == HOST_EID)
			panic("Error: enclave cannot be exited from host!\n");

		u8 status = __get_enclave_status(current_eid);
		if (status != ENCLAVE_RUN)
			panic("Error: Enclave not running!\n");

		__enclave_switch(current_eid, current_tid,
			HOST_EID, 0UL, regs);
		__exit_enclave(current_eid);

		regs->a0 = ret;	// host context

		STOP_TIMER(clean_up, current_eid);
		STOP_TIMER(total, current_eid);
	}
	sbi_debug("ebi_exit_handler finished\n");	
	spin_unlock_enclave(current_eid);
	return 0;
}
