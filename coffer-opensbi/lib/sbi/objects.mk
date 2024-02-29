#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (c) 2019 Western Digital Corporation or its affiliates.
#
# Authors:
#   Anup Patel <anup.patel@wdc.com>
#

libsbi-objs-y += riscv_asm.o
libsbi-objs-y += riscv_atomic.o
libsbi-objs-y += riscv_hardfp.o
libsbi-objs-y += riscv_locks.o

libsbi-objs-y += sbi_bitmap.o
libsbi-objs-y += sbi_bitops.o
libsbi-objs-y += sbi_console.o
libsbi-objs-y += sbi_domain.o
libsbi-objs-y += sbi_ecall.o
libsbi-objs-y += sbi_ecall_base.o
libsbi-objs-y += sbi_ecall_hsm.o
libsbi-objs-y += sbi_ecall_legacy.o
libsbi-objs-y += sbi_ecall_replace.o
libsbi-objs-y += sbi_ecall_vendor.o
libsbi-objs-y += sbi_emulate_csr.o
libsbi-objs-y += sbi_fifo.o
libsbi-objs-y += sbi_hart.o
libsbi-objs-y += sbi_math.o
libsbi-objs-y += sbi_hfence.o
libsbi-objs-y += sbi_hsm.o
libsbi-objs-y += sbi_illegal_insn.o
libsbi-objs-y += sbi_init.o
libsbi-objs-y += sbi_ipi.o
libsbi-objs-y += sbi_misaligned_ldst.o
libsbi-objs-y += sbi_platform.o
libsbi-objs-y += sbi_scratch.o
libsbi-objs-y += sbi_string.o
libsbi-objs-y += sbi_system.o
libsbi-objs-y += sbi_timer.o
libsbi-objs-y += sbi_tlb.o
libsbi-objs-y += sbi_trap.o
libsbi-objs-y += sbi_unpriv.o
libsbi-objs-y += sbi_expected_trap.o

libsbi-objs-y += sbi_ecall_ebi.o
libsbi-objs-y += ebi/sm.o
libsbi-objs-y += ebi/enclave/enclave.o
libsbi-objs-y += ebi/memory/partition_pool.o
libsbi-objs-y += ebi/memory/memory_util.o
libsbi-objs-y += ebi/memory/memngr.o
libsbi-objs-y += ebi/memory/region.o
libsbi-objs-y += ebi/message/message_channel.o
libsbi-objs-y += ebi/debug/debug.o
libsbi-objs-y += ebi/pmp/lpmp.o
libsbi-objs-y += ebi/eval/eval.o
libsbi-objs-y += ebi/ipi.o
# libsbi-objs-y += ebi/memory/memngr.o
# libsbi-objs-y += ebi/memory/mempool.o
libsbi-objs-y += ebi/iomngr/iomngr.o
libsbi-objs-y += ebi/tklock.o
