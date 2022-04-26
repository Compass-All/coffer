#include "ramfs.h"
#include "../dependency.h"
#include <errno.h>

#include "../vfs/vnode.h"
#include "../vfs/mount.h"
#include "../vfs/dentry.h"

extern struct vnops ramfs_vnops;

static int ramfs_mount(struct mount *mp, const char *dev, int flags,
		       const void *data);

static int ramfs_unmount(struct mount *mp, int flags);

#define ramfs_sync    ((vfsop_sync_t)vfscore_nullop)
#define ramfs_vget    ((vfsop_vget_t)vfscore_nullop)
#define ramfs_statfs    ((vfsop_statfs_t)vfscore_nullop)

/*
 * File system operations
 */
struct vfsops ramfs_vfsops;

static const char fs_name[] = "ramfs";
static struct vfscore_fs_type fs_ramfs;

/*
 * Mount a file system.
 */
static int
ramfs_mount(struct mount *mp, const char *dev __unused,
	    int flags __unused, const void *data __unused)
{
	struct ramfs_node *np;

	debug("%s: dev=%s\n", __func__, dev);

	/* Create a root node */
	np = ramfs_allocate_node("/", VDIR);
	if (np == NULL)
		return ENOMEM;
	mp->m_root->d_vnode->v_data = np;
	return 0;
}

/*
 * Unmount a file system.
 *
 * NOTE: Currently, we don't support unmounting of the RAMFS. This is
 *       because we have to deallocate all nodes included in all sub
 *       directories, and it requires more work...
 */
static int
ramfs_unmount(struct mount *mp, int flags __unused)
{
	vfscore_release_mp_dentries(mp);
	return 0;
}

void init_ramfs()
{
	ramfs_vfsops.vfs_mount		= ramfs_mount;
	ramfs_vfsops.vfs_unmount	= ramfs_unmount;
	ramfs_vfsops.vfs_sync		= ramfs_sync;
	ramfs_vfsops.vfs_vget		= ramfs_vget;
	ramfs_vfsops.vfs_statfs		= ramfs_statfs;
	ramfs_vfsops.vfs_vnops		= &ramfs_vnops;

	ramfs_vnops_init();

	fs_ramfs.vs_name 	= &fs_name[0];
	fs_ramfs.vs_init 	= NULL;
	fs_ramfs.vs_op		= &ramfs_vfsops;

	register_fs(&fs_ramfs);
}