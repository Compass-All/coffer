#include "sbi/sbi_types.h"
#include <enclave/eid.h>
#include <memory/memory.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/ebi/enclave.h>
#include <sbi/ebi/memory_util.h>
#include <sbi/ebi/iomngr.h>
#include <sbi/ebi/ipi.h>
#include <sbi/ebi/pmp.h>
#include <sbi/ebi/region.h>
#include <sbi/ebi/tklock.h>
#include <sbi/riscv_atomic.h>
#include <sbi/riscv_locks.h>
#include <sbi/sbi_list.h>
#include <sbi/sbi_string.h>
#include <sbi/sbi_system.h>
#include <sbi/sbi_timer.h>
#include <stdbool.h>
#include <types.h>
#include <util/gnu_attribute.h>
#include <message/short_message.h>

typedef struct {
    u64 id;

    paddr_t pa_start;
    size_t size;
    bool sharable;

    volatile u64 holder;
    atomic_t num_waiting;

    struct sbi_dlist entry;

    tklock_t lock;
} device_t;

static spinlock_t device_lock;
static SBI_LIST_HEAD(device_list);
static uint64_t device_num = 0;

static device_t* enc_holding_device[NUM_ENCLAVE + 1];

static struct sbi_dlist* get_device_list(void)
{
    spin_lock(&device_lock);
    return &device_list;
}

static void release_device_list(void)
{
    spin_unlock(&device_lock);
}

static void hold_device(u64 eid, device_t* d)
{
    atomic_add_return(&d->num_waiting, 1);
    // tklock_fetch(&d->lock);
    atomic_sub_return(&d->num_waiting, 1);
    d->holder = eid;
    enc_holding_device[eid] = d;
    enclave_add_region(eid, d->pa_start, d->size, 0);
}

static void release_device(u64 eid)
{
    device_t* d = enc_holding_device[eid];
    enc_holding_device[eid] = NULL;
    d->holder = HOST_EID;
    // tklock_release(&d->lock);
    __enclave_remove_region(eid, d->pa_start);
}

void clear_device(u64 eid)
{
    get_device_list();
    if (enc_holding_device[eid]) {
        release_device(eid);
    }
    // pmp_disable(NUM_REGIONS);
    release_device_list();
}

static int device_register(device_t* device)
{
    device_t* d;

    if (!device) {
        return EBI_ERROR;
    }

    sbi_list_for_each_entry(d, &device_list, entry)
    {
        if (device->pa_start < d->pa_start + d->size
            && d->pa_start < device->pa_start + device->size) {
            // Overlap!
            return EBI_ERROR;
        }
    }

    device->id = device_num++;
    atomic_write(&device->num_waiting, 0);
    SBI_INIT_LIST_HEAD(&device->entry);
    // SBI_INIT_LIST_HEAD(&device->waiting_list);
    sbi_list_add_tail(&device->entry, &device_list);
    tklock_init(&device->lock);

    return EBI_OK;
}

static device_t* find_matching_device(paddr_t pa)
{
    device_t *d, *ret = NULL;
    sbi_debug("addr: %lx\n", pa);

    spin_lock(&device_lock);
    sbi_list_for_each_entry(d, &device_list, entry)
    {
        sbi_debug("[%lu] d->pa_start: %lx, d->size: %lx\n", d->id, d->pa_start, d->size);
        if (pa >= d->pa_start && pa < d->pa_start + d->size) {
            ret = d;
            break;
        }
    }
    spin_unlock(&device_lock);

    return ret;
}

void device_update_permission(u64 eid)
{
    volatile device_t* d;
    if (eid != get_current_eid()) {
        show(eid);
        panic("Cannot update device permission for offline enclave!\n");
    }
    d = enc_holding_device[eid];
    if (d) {
        sbi_debug("Enc #%lu holding device #%lu\n", eid, d->id);
        // sbi_DEBUG("Device #%lu holder #%lu\n", d->id, d->holder);
        if (d->holder != eid) {
            panic("EID not match!\n");
        }
    }
}

////// DEBUG //////
// device_t test = {
//     .pa_start = 0x140000000,
//     .size = 0x200000,
//     .sharable = false,
// };

device_t uart = {
#if defined __QEMU__
    .pa_start = 0x10000000,
#elif defined __UNMATCHED__
    .pa_start = 0x10011000,
#else
#error "unknown platform"
#endif
    .size = 0x1000,
};

device_t timer = {
#if defined __QEMU__
    .pa_start = 0x101000UL,
    .size = 0x1000,
#elif defined __UNMATCHED__
    .pa_start = 0x200B000UL,
    .size = 0x1000,
    .sharable = false,
#else
#error "unknown platform"
#endif
};

// TODO: Size from config file
device_t pcie_ecam = {
#if defined __QEMU__
    .pa_start = 0x30000000,
    .size = 0x10000000,
#elif defined __UNMATCHED__
    .pa_start = 0,
    .size = 0,
#else
#error "unknown platform"
#endif
};

device_t pcie_pio = {
#if defined __QEMU__
    .pa_start = 0x3000000,
    .size = 0x10000,
#elif defined __UNMATCHED__
    .pa_start = 0,
    .size = 0,
#else
#error "unknown platform"
#endif
};

#define NUM_DEV 4
static device_t *dev_list[NUM_DEV] = {
    // &test, 
    &uart, &timer, &pcie_ecam, &pcie_pio
};
//////       //////

int init_device(void)
{
    int rc = 0;

    u64* sifive_mtimecmp = (void*)0x02004000;
    sifive_mtimecmp[0] = 5000;

    sbi_memset(enc_holding_device, 0, sizeof(enc_holding_device));
    // TODO register device here

    for (int i = 0; i < NUM_DEV; i++) {
        rc = device_register(dev_list[i]);
        if (rc)
            return rc;
    }

    SPIN_LOCK_INIT(&device_lock);
    return rc;
}

// // todo: move this function to a separate file
// static void enclave_schedule(struct sbi_trap_regs* regs)
// {
//     if (get_current_eid() == HOST_EID)
//         return;

//     u64 smode = csr_read(CSR_MSTATUS);
// 	smode = (smode & MSTATUS_MPP) >> MSTATUS_MPP_SHIFT;
//     sbi_printf("eid = %lu, smode = %lu\n", get_current_eid(), smode);
//     if (smode)
//         return;

//     regs->a0 = TIMER_INTERRUPT;
//     ebi_suspend_handler(regs);
// }

void io_schedule(struct sbi_trap_regs* regs)
{
    struct sbi_dlist* device_list = get_device_list();
    device_t* d;

    int released = 0;
    sbi_list_for_each_entry(d, device_list, entry)
    {
        if (!d->sharable) {
            continue;
        }
        if (atomic_read(&d->num_waiting) != 0) {
            sbi_debug("Enc #%lu releasing device #%lu\n", d->holder, d->id);
            release_device(d->holder);
            released = 1;
            flush_tlb();
        }
    }
    if (released)
        ipi_send_ebi_update_pmp(-1);
    release_device_list();

    // enclave_schedule(regs);
}

int match_device_for_enclave(u64 eid, paddr_t pa)
{
    int rc = 0;

    if (likely(pa >= PHYS_MEM_START && pa < PHYS_MEM_END))
        return rc;

    device_t* device;
    sbi_debug("Checkpoint pa=0x%lx\n", pa);
    device = find_matching_device(pa);
    show(device);
    if (device) {
        sbi_debug("#%lu waiting for device %lu, current holder: %lu\n", eid, device->id, device->holder);
        hold_device(eid, device);
        sbi_debug("#%lu holds device %lu\n", eid, device->id);
        // device_update_permission(eid);
        rc = 1;
    }
    show(rc);
    return rc;
}