#pragma once

#include <enclave/enclave_ops.h>

u64 suspend_enclave();
u64 suspend_enclave_with_message(u64 short_message);