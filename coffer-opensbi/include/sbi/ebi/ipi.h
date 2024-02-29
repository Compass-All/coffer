#ifndef EBI_IPI_H
#define EBI_IPI_H

#include <sbi/sbi_ipi.h>

#define EBI_DECLARE_IPI_FUNC(event_name) int ipi_send_ebi_##event_name(ulong mask);

EBI_DECLARE_IPI_FUNC(postboot_init)
EBI_DECLARE_IPI_FUNC(wait_for_compaction)
EBI_DECLARE_IPI_FUNC(update_pmp)
EBI_DECLARE_IPI_FUNC(log_pc)

#undef EBI_DECLARE_IPI_FUNC

int init_ipi(void);

#endif // !EBI_IPI_H