#include <enclave/eid.h>
#include <memory/page_table.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/ebi/enclave.h>
#include <sbi/ebi/iomngr.h>
#include <sbi/ebi/ipi.h>
#include <sbi/ebi/partition_pool.h>
#include <sbi/ebi/pmp.h>
#include <sbi/riscv_asm.h>
#include <sbi/sbi_timer.h>
#include <util/gnu_attribute.h>

#define EBI_DEFINE_IPI_EVENT(event_name)                              \
    static uint32_t ipi_ebi_##event_name##_event = SBI_IPI_EVENT_MAX; \
    static void process_ipi_ebi_##event_name(struct sbi_scratch* scratch, struct sbi_trap_regs* regs)

#define EBI_DEFINE_IPI_OPS(ops_name, event_name)                   \
    static struct sbi_ipi_event_ops ipi_ebi_##event_name##_ops = { \
        .name = ops_name,                                          \
        .process = process_ipi_ebi_##event_name,                   \
    };

#define EBI_DEFINE_IPI_FUNC(event_name)                                         \
    int ipi_send_ebi_##event_name(ulong hmask)                                  \
    {                                                                           \
        return sbi_ipi_send_many(hmask, 0, ipi_ebi_##event_name##_event, NULL); \
    }

EBI_DEFINE_IPI_EVENT(postboot_init)
{
    sbi_debug("Hart #%u received ipi_ebi_postboot_init\n", current_hartid());
    // sbi_timer_set_delta(50000);
    activate_lpmp(get_current_eid());
	asm volatile("sfence.vma");
}

EBI_DEFINE_IPI_EVENT(wait_for_compaction)
{
    // sbi_DEBUG("waiting\n");
    wait_for_compaction();
    // sbi_DEBUG("wait done\n");
}

EBI_DEFINE_IPI_EVENT(update_pmp)
{
    // sbi_debug("Hart #%u received ipi_`ebi_update_pmp\n", current_hartid());
    // device_update_permission(get_current_eid());
    activate_lpmp(get_current_eid());
    flush_tlb();
}

EBI_DEFINE_IPI_EVENT(log_pc)
{
    static u8 dumped = 0;

    if (dumped)
        return;
    
    __unused u32 hartid = current_hartid();
    __unused volatile u64* sifive_mtimecmp = (void*)0x02004000;
    __unused u64 compare = sifive_mtimecmp[hartid];
    __unused u64 now = csr_read(time);

    if (now > compare && now - compare > 5000000UL) {
        __unused u64 mstatus = regs->mstatus;
        __unused u64 mepc = regs->mepc;
        __unused u64 mcause = csr_read(CSR_MCAUSE);
        __unused u64 mpp = (mstatus & MSTATUS_MPP) >> MSTATUS_MPP_SHIFT;

        __unused u64 sepc = csr_read(CSR_SEPC);
        __unused u64 satp = csr_read(CSR_SATP);
        __unused u64 stvec = csr_read(CSR_STVEC);

        __unused u64 eid = get_current_eid();

        LOG(eid);
        LOG(mepc);
        LOG(mpp);
        LOG(mstatus);
        LOG(mcause);
        LOG(sepc);
        LOG(satp);
        LOG(stvec);

        // DUMP_enclave_context(eid);
        dumped = 1;
    }
}

EBI_DEFINE_IPI_FUNC(postboot_init)
EBI_DEFINE_IPI_FUNC(wait_for_compaction)
EBI_DEFINE_IPI_FUNC(update_pmp)
EBI_DEFINE_IPI_FUNC(log_pc)

EBI_DEFINE_IPI_OPS("IPI_EBI_POSTBOOT_INIT", postboot_init)
EBI_DEFINE_IPI_OPS("IPI_EBI_WAIT_FOR_COMPACTION", wait_for_compaction);
EBI_DEFINE_IPI_OPS("IPI_EBI_UPDATE_PMP", update_pmp)
EBI_DEFINE_IPI_OPS("IPI_EBI_LOG_PC", log_pc)

int init_ipi(void)
{
    int rc = 0;
#define EBI_CREATE_IPI_EVENT(rc, event_name)                      \
    do {                                                          \
        (rc) = sbi_ipi_event_create(&ipi_ebi_##event_name##_ops); \
        if ((rc) < 0)                                             \
            return rc;                                            \
        ipi_ebi_##event_name##_event = (rc);                      \
    } while (0)

    EBI_CREATE_IPI_EVENT(rc, postboot_init);
    EBI_CREATE_IPI_EVENT(rc, wait_for_compaction);
    EBI_CREATE_IPI_EVENT(rc, update_pmp);
    EBI_CREATE_IPI_EVENT(rc, log_pc);

    sbi_debug("send postboot_init\n");
    ipi_send_ebi_postboot_init(-1);

    return 0;
}