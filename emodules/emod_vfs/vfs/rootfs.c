#include "rootfs.h"
#include "mount.h"
#include "../dependency.h"

static const char *rootfs = "ramfs";
static const char *rootdev  = "";
static const char *rootopts = "";
static __u64 rootflags;

int vfscore_rootfs(void)
{
	/*
	 * Initialization of the root filesystem '/'
	 * NOTE: Any additional sub mount points (like '/dev' with devfs)
	 * have to be mounted later.
	 */
	if (!rootfs || rootfs[0] == '\0') {
		debug("Parameter 'vfs.rootfs' is invalid\n");
		return -1;
	}

	debug("Mount %s to /...\n", rootfs);
	if (mount(rootdev, "/", rootfs, rootflags, rootopts) != 0) {
		panic("Failed to mount rootfs\n");
		return -1;
	}

	return 0;
}