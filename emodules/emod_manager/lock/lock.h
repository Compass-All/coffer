/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2019 Western Digital Corporation or its affiliates.
 *
 * Authors:
 *   Anup Patel <anup.patel@wdc.com>
 */

#ifndef __EMOD_MANAGER_LOCKS_H__
#define __EMOD_MANAGER_LOCKS_H__

typedef struct {
	volatile long lock;
} spinlock_t;

extern spinlock_t grand_lock;
extern spinlock_t log_lock;

#define __RISCV_SPIN_UNLOCKED 0

#define SPIN_LOCK_INIT(_lptr) (_lptr)->lock = __RISCV_SPIN_UNLOCKED

#define SPIN_LOCK_INITIALIZER                  \
	{                                      \
		.lock = __RISCV_SPIN_UNLOCKED, \
	}

int spin_lock_check(spinlock_t *lock);

int spin_trylock(spinlock_t *lock);

void spin_lock(spinlock_t *lock);

void spin_unlock(spinlock_t *lock);

void spin_lock_log();
void spin_unlock_log();

int spin_trylock_grand();
void spin_unlock_grand();

#endif
