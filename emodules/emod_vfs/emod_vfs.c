#include <emodules/emod_vfs/emod_vfs.h>
#include "dependency.h"
#include "vfs/fd.h"
#include "vfs/syscall_handlers.h"
#include "vfs/rootfs.h"
#include "ramfs/ramfs.h"
#include "vfs/mount.h"
#include "mmap/mman.h"

// ---------------
// emodule vfs descriptor

static emod_desc_t emod_vfs_desc = {
	.emod_id = EMODULE_ID_VFS,
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

static void init_functions()
{
	debug("initializing vfs\n");
	mount_init();
	init_ramfs();
	fdtable_init();
	vfscore_init();
	vfscore_rootfs();
}

static void init_dependency()
{
	vaddr_t emod_debug_getter = emod_manager.emod_manager_api
		.acquire_emodule(EMODULE_ID_DEBUG);
	emod_debug_t (*get_emod_debug)(void) =
		(void *)emod_debug_getter;
	emod_debug = get_emod_debug();

	vaddr_t emod_alloc_getter = emod_manager.emod_manager_api
		.acquire_emodule(EMODULE_ID_ALLOC);
	emod_alloc_t (*get_emod_alloc)(void) =
		(void *)emod_alloc_getter;
	emod_alloc = get_emod_alloc();

	debug("Hello from init_dependency in emod_vfs\n");
}

__attribute__((section(".text.init")))
vaddr_t vfs_init(vaddr_t emod_manager_getter)
{
	// init api
	emod_vfs_api.syscall_handler_open	 	= &syscall_handler_open;
	emod_vfs_api.syscall_handler_openat 	= &syscall_handler_openat;
	emod_vfs_api.syscall_handler_close 		= &syscall_handler_close;
	emod_vfs_api.syscall_handler_lseek 		= &syscall_handler_lseek;
	emod_vfs_api.syscall_handler_preadv 	= &syscall_handler_preadv;
	emod_vfs_api.syscall_handler_pread64 	= &syscall_handler_pread64;
	emod_vfs_api.syscall_handler_readv 		= &syscall_handler_readv;
	emod_vfs_api.syscall_handler_read 		= &syscall_handler_read;
	emod_vfs_api.syscall_handler_pwritev 	= &syscall_handler_pwritev;
	emod_vfs_api.syscall_handler_pwrite64 	= &syscall_handler_pwrite64;
	emod_vfs_api.syscall_handler_writev 	= &syscall_handler_writev;
	emod_vfs_api.syscall_handler_write 		= &syscall_handler_write;
	emod_vfs_api.syscall_handler_fstat 		= &syscall_handler_fstat;
	emod_vfs_api.syscall_handler_fstatat 	= &syscall_handler_fstatat;
	emod_vfs_api.syscall_handler_mkdirat 	= &syscall_handler_mkdirat;
	emod_vfs_api.syscall_handler_fcntl	 	= &syscall_handler_fcntl;
	emod_vfs_api.syscall_handler_getdents	= &syscall_handler_getdents;
	emod_vfs_api.syscall_handler_getcwd		= &syscall_handler_getcwd;
	emod_vfs_api.syscall_handler_fsync		= &syscall_handler_fsync;
	emod_vfs_api.syscall_handler_unlinkat	= &syscall_handler_unlinkat;
	emod_vfs_api.syscall_handler_ftruncate	= &syscall_handler_ftruncate;
	emod_vfs_api.syscall_handler_access		= &syscall_handler_access;
	emod_vfs_api.syscall_handler_faccessat	= &syscall_handler_faccessat;
	emod_vfs_api.syscall_handler_mmap	 	= &mmap;
	emod_vfs_api.syscall_handler_munmap	 	= &munmap;

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

	init_dependency();
	init_functions();

	debug("end of vfs init\n");

	return (vaddr_t)get_emod_vfs;
}