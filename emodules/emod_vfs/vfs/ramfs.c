#include "ramfs.h"
#include "mount.h"
#include "../dependency.h"

// todo

static const char fs_name[] = "ramfs";

static struct vfscore_fs_type fs_ramfs;
//  = {
// 	.vs_name = "ramfs",
// 	.vs_init = NULL,
// 	.vs_op = NULL,
// 	// .vs_op = &ramfs_vfsops,
// };

// init: register ramfs

void init_ramfs()
{
	fs_ramfs.vs_name = &fs_name[0];
	// init
	// op

	register_fs(&fs_ramfs);
}