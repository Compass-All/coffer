#include "timestamp.h"

#include "../dependency.h"
#include "memory/page_table.h"

/* these code are duplicated from emod_manager.
 * todo: setup a timer emodule
 */

#if defined __QEMU__
#define MTIME_PA_ALIGNED	0x101000UL
#define MTIME_PA_OFFSET		0x0
#define FREQ				1000000000UL
#elif defined __UNMATCHED__
#define MTIME_PA_ALIGNED	0x200B000UL
#define MTIME_PA_OFFSET		0xFF8
#define FREQ 				1000000UL
#else
#error "unsupported platform"
#endif

static vaddr_t time_csr_va = 0x600000000;

void emod_net_timestamp_init()
{
    paddr_t pa = MTIME_PA_ALIGNED;
	map_page(time_csr_va, pa, PTE_R, SV39_LEVEL_PAGE);
}

void emod_net_timestamp_get(emod_net_timestamp *ts)
{
    u64 offset = MTIME_PA_OFFSET;
    u64 time = *(volatile u64 *)(time_csr_va + offset);
    *ts = (emod_net_timestamp)time;
}

s64 emod_net_timestamp_diff(emod_net_timestamp *ts1,
        emod_net_timestamp *ts2)
{
    return *ts2 - *ts1;
}

void sleep(u32 ms)
{
    emod_net_timestamp ts1, ts2;
    emod_net_timestamp_get(&ts1);
    while (1) {
        emod_net_timestamp_get(&ts2);
        if (emod_net_timestamp_diff(&ts1, &ts2) >= ms * 1000)
            break;
    }
}