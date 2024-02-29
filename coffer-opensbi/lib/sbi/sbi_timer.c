/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Anup Patel <anup.patel@wdc.com>
 */

#include <sbi/riscv_asm.h>
#include <sbi/riscv_encoding.h>
#include <sbi/sbi_error.h>
#include <sbi/sbi_hart.h>
#include <sbi/sbi_platform.h>
#include <sbi/sbi_scratch.h>
#include <sbi/sbi_timer.h>
#include <sbi/ebi/iomngr.h>
#include <sbi/ebi/ebi_debug.h>
#include <sbi/ebi/enclave.h>
#include <sbi/ebi/partition_pool.h>
#include <enclave/eid.h>
#include <sbi/ebi/ipi.h>

static unsigned long time_delta_off;
static u64 (*get_time_val)(const struct sbi_platform *plat);

#if __riscv_xlen == 32
static u64 get_ticks(const struct sbi_platform *plat)
{
	u32 lo, hi, tmp;
	__asm__ __volatile__("1:\n"
			     "rdtimeh %0\n"
			     "rdtime %1\n"
			     "rdtimeh %2\n"
			     "bne %0, %2, 1b"
			     : "=&r"(hi), "=&r"(lo), "=&r"(tmp));
	return ((u64)hi << 32) | lo;
}
#else
static u64 get_ticks(const struct sbi_platform *plat)
{
	unsigned long n;

	__asm__ __volatile__("rdtime %0" : "=r"(n));
	return n;
}
#endif

u64 sbi_timer_value(void)
{
	return get_time_val(sbi_platform_thishart_ptr());
}

u64 sbi_timer_virt_value(void)
{
	u64 *time_delta = sbi_scratch_offset_ptr(sbi_scratch_thishart_ptr(),
						 time_delta_off);

	return sbi_timer_value() + *time_delta;
}

u64 sbi_timer_get_delta(void)
{
	u64 *time_delta = sbi_scratch_offset_ptr(sbi_scratch_thishart_ptr(),
						 time_delta_off);

	return *time_delta;
}

void sbi_timer_set_delta(ulong delta)
{
	u64 *time_delta = sbi_scratch_offset_ptr(sbi_scratch_thishart_ptr(),
						 time_delta_off);

	*time_delta = (u64)delta;
}

void sbi_timer_set_delta_upper(ulong delta_upper)
{
	u64 *time_delta = sbi_scratch_offset_ptr(sbi_scratch_thishart_ptr(),
						 time_delta_off);

	*time_delta &= 0xffffffffULL;
	*time_delta |= ((u64)delta_upper << 32);
}

void sbi_timer_event_start(u64 next_event)
{
#ifdef __UNMATCHED__
	u64 eid = get_current_eid();
	static u64 count[NUM_ENCLAVE + 1] = { 0 };
	volatile u64* sifive_mtimecmp = (void*)0x02004000;

	if ((eid == HOST_EID && count[HOST_EID] % 10000 == 0)
	|| (eid != HOST_EID && count[eid] % 100 == 0)) {
		// sbi_debug("Enclave%lu setting next event: %lu "
		// 	"(timecmp: %lu\t%lu\t%lu\t%lu)\n",
		// 	eid, next_event,
		// 	sifive_mtimecmp[1],
		// 	sifive_mtimecmp[2],
		// 	sifive_mtimecmp[3],
		// 	sifive_mtimecmp[4]
		// );
		;
	}
	count[eid]++;

	if (count[HOST_EID] % 10000 == 0) {
		u64 now = csr_read(time);
		for (int i = 1; i <= 4; i++) {
			if (now > sifive_mtimecmp[i]
			&& now - sifive_mtimecmp[i] > 50000000UL) {
				sbi_DEBUG("sending IPI\n");
				ipi_send_ebi_log_pc(-1);
				dump_enclave_status();
			}
		}
	}
#endif

	sbi_platform_timer_event_start(sbi_platform_thishart_ptr(), next_event);
	csr_clear(CSR_MIP, MIP_STIP);
	csr_set(CSR_MIE, MIP_MTIP);
}

void sbi_timer_process(struct sbi_trap_regs* regs)
{
	// static ulong interval = 5000000UL;

	// static ulong lasttime = 0;
	// ulong now = csr_read(time);
	// if (now - lasttime >= interval) {
    	// io_schedule(regs);
		// lasttime = now;
	// }
	csr_clear(CSR_MIE, MIP_MTIP);
	csr_set(CSR_MIP, MIP_STIP);
}

int sbi_timer_init(struct sbi_scratch *scratch, bool cold_boot)
{
	u64 *time_delta;
	const struct sbi_platform *plat = sbi_platform_ptr(scratch);
	int ret;

	if (cold_boot) {
		time_delta_off = sbi_scratch_alloc_offset(sizeof(*time_delta),
							  "TIME_DELTA");
		if (!time_delta_off)
			return SBI_ENOMEM;
	} else {
		if (!time_delta_off)
			return SBI_ENOMEM;
	}

	time_delta = sbi_scratch_offset_ptr(scratch, time_delta_off);
	*time_delta = 0;

	ret = sbi_platform_timer_init(plat, cold_boot);
	if (ret)
		return ret;

	if (sbi_hart_has_feature(scratch, SBI_HART_HAS_TIME))
		get_time_val = get_ticks;
	else if (sbi_platform_has_timer_value(plat))
		get_time_val = sbi_platform_timer_value;
	else
		/* There is no method to provide timer value */
		return SBI_ENODEV;

	return 0;
}

void sbi_timer_exit(struct sbi_scratch *scratch)
{
	sbi_platform_timer_event_stop(sbi_platform_ptr(scratch));

	csr_clear(CSR_MIP, MIP_STIP);
	csr_clear(CSR_MIE, MIP_MTIP);

	sbi_platform_timer_exit(sbi_platform_ptr(scratch));
}
