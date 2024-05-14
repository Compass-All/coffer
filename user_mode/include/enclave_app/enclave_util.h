#pragma once

#include <enclave/enclave_ops.h>

// u64 suspend_enclave();
// u64 suspend_enclave_with_message(u64 short_message);

__inline u64 suspend_enclave()
{
	return __ecall_ebi_suspend(1UL);
}

__inline u64 suspend_enclave_with_message(u64 short_message)
{
	return __ecall_ebi_suspend(short_message);
}