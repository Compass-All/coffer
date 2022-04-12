#include <emodules/emod_vfs/emod_vfs.h>
#include "dependency.h"

// ---------------
// emodule vfs descriptor

static emod_desc_t emod_vfs_desc = {
	.emod_id = EMODULE_ID_DUMMY,
	.name = "emodule vfs",
	.__signature = 0
};
static emod_vfs_api_t emod_vfs_api;
static emod_vfs_t emod_vfs;

// ---------------
// emod_vfs init and getter
static emod_vfs_t get_emod_vfs()
{
	return emod_vfs;
}

__attribute__((section(".text.init")))
vaddr_t vfs_init(vaddr_t emod_manager_getter)
{
	// init api
	// emod_vfs_api = (emod_vfs_api_t) {
	// 	.sys_open = sys_open,
	// 	.sys_read = sys_read
	// };

	// init emodule
	emod_vfs = (emod_vfs_t) {
		.emod_vfs_desc = emod_vfs_desc,
		.emod_vfs_api = emod_vfs_api
	};

	// record emod_manager
	if (emod_manager_getter == (vaddr_t)0UL) {
		return (vaddr_t)0UL;
	}

	emod_manager_t (*get_emod_manager)(void) = 
		(void *)emod_manager_getter;
	emod_manager = get_emod_manager();

	emod_manager.emod_manager_api.test();

	return (vaddr_t)get_emod_vfs;
}