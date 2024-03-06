#pragma once

#include <enclave/enclave_ops.h>
#include <message/short_message.h>

#ifdef IN_EMOD_MANAGER
#include "../../emodules/emod_manager/printf/printf.h"
#include "../../emodules/emod_manager/lock/lock.h"
#define try_lock spin_trylock_grand
#define unlock spin_unlock_grand
#else
#include <emodules/dependency.h>
#define try_lock emm_spin_trylock_grand
#define unlock emm_spin_unlock_grand
#endif

#define spin_lock_grand_suspend()                                        \
  ({                                                                           \
    info("Try to get grand lock\n");                                           \
    while (!try_lock()) {                                                         \
      __ecall_ebi_suspend(BLOCKED);                                            \
    }                                                                          \
    info("Got grand lock\n");                                                  \
  })

#define spin_unlock_grand()                                                    \
  ({                                                                           \
    info("Release grand lock\n");                                              \
    unlock();                                                                \
  })
