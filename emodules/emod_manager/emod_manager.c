#include <emodules/emod_manager/emod_manager.h>
#include <emodules/emodule_info.h>
#include <message/message.h>
#include <enclave/host_ops.h>
#include <enclave/enclave_ops.h>
#include "printf/debug.h"

#include <emodules/emod_dummy/emod_dummy.h>

emod_dummy_api_t emod_dummy_api;

void load_emod_dummy()
{
	/**
	 * vaddr_t emod_dummy = ...;
	 * 
	 * void (*emod_dummy_init)(emod_dummy_api_t *);
	 * emod_dummy_init = (void *)emod_dummy;
	 * // set function pointers
	 * emod_dummy_init(&emod_dummy_api);
	 * 
	 * emod_dummy_api->dummy_funt1(arg1=3, arg2=5);
	 * 
	 */
}