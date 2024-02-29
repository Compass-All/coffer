#include <enclave_util.h>
#include <types.h>
#include <stdio.h>

u64 suspend_enclave()
{
	return __ecall_ebi_suspend(1UL);
}

u64 suspend_enclave_with_message(u64 short_message)
{
	return __ecall_ebi_suspend(short_message);
}