#include "mount.h"
#include <util/list.h>
#include "../dependency.h"
#include "string.h"
#include "errno.h"
#include "prex.h"
#include "lookup.h"

UK_LIST_DECLARE(mount_list);

void vfs_busy(struct mount *mp)
{
	/* The m_count is not really checked anywhere
	 * currently. Atomic is enough. But it could be that obtaining
	 * mount_lock will be needed in the future */
	mp->m_count++;
}

void vfs_unbusy(struct mount *mp)
{
	/* The m_count is not really checked anywhere
	 * currently. Atomic is enough. But it could be that obtaining
	 * mount_lock will be needed in the future */
	// ukarch_dec(&mp->m_count);
	mp->m_count--;
}

#define FS_LIST_LEN	5
static u32 fs_cnt = 0;
static struct vfscore_fs_type fs_list[FS_LIST_LEN];

#define for_each_fs(iter)					\
	for (iter = &fs_list[0];				\
	     iter < &fs_list[FS_LIST_LEN];		\
	     iter++)

void register_fs(struct vfscore_fs_type *fs)
{
	fs_list[fs_cnt].vs_name = fs->vs_name;
	fs_list[fs_cnt].vs_init = fs->vs_init;
	fs_list[fs_cnt].vs_op	= fs->vs_op;

    show(fs_cnt);
	show(fs->vs_name);
	show(fs->vs_init);
	show(fs->vs_op);

	if (fs->vs_name)
		debug("name: %s\n", fs->vs_name);

	fs_cnt++;
}

/*
 * Lookup file system.
 */
static const struct vfscore_fs_type *fs_getfs(const char *name)
{
	const struct vfscore_fs_type *fs = NULL;

	for_each_fs(fs) {
		show(fs);

		if (!fs || !fs->vs_name)
			continue;

		show(fs->vs_name);
		show(fs->vs_init);
		show(fs->vs_op);

		if (strncmp(name, fs->vs_name, FSMAXNAMES) == 0)
			return fs;
	}

	debug("fs not found\n");

	return NULL;
}

static size_t count_match(const char *path, char *mount_root)
{
	size_t len = 0;

	while (*path && *mount_root) {
		if (*path != *mount_root)
			break;

		path++;
		mount_root++;
		len++;
	}
	if (*mount_root != '\0')
		return 0;

	if (len == 1 && *(path - 1) == '/')
		return 1;

	if (*path == '\0' || *path == '/')
		return len;
	return 0;
}

int vfs_findroot(const char *path, struct mount **mp, char **root)
{
	struct mount *m = NULL, *tmp;
	size_t len, max_len = 0;

	if (!path)
		return -1;

	/* Find mount point from nearest path */
	uk_list_for_each_entry(tmp, &mount_list, mnt_list) {
		len = count_match(path, tmp->m_path);
		if (len > max_len) {
			max_len = len;
			m = tmp;
		}
	}
	if (m == NULL)
		return -1;
	*root = (char *)(path + max_len);
	if (**root == '/')
		(*root)++;
	*mp = m;
	return 0;
}

int vfscore_nullop()
{
	return 0;
}

int vfscore_einval()
{
	return EINVAL;
}

int device_open(const char *name __unused, int mode __unused,
		struct device **devp __unused)
{
	panic("device_open not implemented\n");
	return 0;
}

int device_close(struct device *dev)
{
	panic("device_close not implemented\n");
	return 0;
}

void vfscore_release_mp_dentries(struct mount *mp)
{
	/* Decrement referece count of root vnode */
	if (mp->m_covered) {
		drele(mp->m_covered);
	}

	/* Release root dentry */
	drele(mp->m_root);
}

int mount(
	const char* 	dev,
	const char* 	dir,
	const char* 	fsname,
	unsigned long	flags,
	const void*		data
)
{
	const struct vfscore_fs_type *fs;
	struct mount *mp;
	struct device *device;
	struct dentry *dp_covered = NULL;
	struct vnode *vp = NULL;
	int error;

	debug("VFS: mounting %s at %s\n", fsname, dir);
	debug("dev: %s\n", dev);
	debug("dir: %s\n", dir);
	debug("fsname: %s\n", fsname);

	if (!dir || *dir == '\0')
		return -ENOENT;

	/* Find a file system. */
	if (!(fs = fs_getfs(fsname)))
		return -ENODEV;  /* No such file system */

	/* Open device. NULL can be specified as a device. */
	// Allow device_open() to fail, in which case dev is interpreted
	// by the file system mount routine (e.g zfs pools)
	device = 0;
	if (dev && strncmp(dev, "/dev/", 5) == 0)
		device_open(dev + 5, DO_RDWR, &device);

	/* Check if device or directory has already been mounted. */
	// We need to avoid the situation where after we already verified that
	// the mount point is free, but before we actually add it to mount_list,
	// another concurrent mount adds it. So we use a new mutex to ensure
	// that only one mount() runs at a time. We cannot reuse the existing
	// mount_lock for this purpose: If we take mount_lock and then do
	// lookups, this is lock order inversion and can result in deadlock.

	/* TODO: protect the function from reentrance, as described in
	 * the comment above */
	/* static mutex sys_mount_lock; */
	/* SCOPE_LOCK(sys_mount_lock); */

	// uk_mutex_lock(&mount_lock);
	show(&mount_list);

	uk_list_for_each_entry(mp, &mount_list, mnt_list) {
		if (!strcmp(mp->m_path, dir) ||
		    (device && mp->m_dev == device)) {
			error = EBUSY;  /* Already mounted */
			// uk_mutex_unlock(&mount_lock);
			goto err1;
		}
	}
	// uk_mutex_unlock(&mount_lock);
	/*
	 * Create VFS mount entry.
	 */
	mp = malloc(sizeof(struct mount));
	if (!mp) {
		error = ENOMEM;
		panic("malloc failed\n");
		goto err1;
	}
	mp->m_count = 0;
	mp->m_op = fs->vs_op;
	mp->m_flags = flags;
	mp->m_dev = device;
	mp->m_data = NULL;
	strlcpy(mp->m_path, dir, sizeof(mp->m_path));
	strlcpy(mp->m_special, dev, sizeof(mp->m_special));

	show(mp);
	show(mp->m_count);
	show(mp->m_op);
	show(mp->m_flags);
	show(mp->m_dev);
	show(mp->m_data);
	show(mp->m_path);
	show(mp->m_special);

	/*
	 * Get vnode to be covered in the upper file system.
	 */
	if (*dir == '/' && *(dir + 1) == '\0') {
		/* Ignore if it mounts to global root directory. */
		dp_covered = NULL;
	} else {
		if ((error = namei(dir, &dp_covered)) != 0) {

			error = ENOENT;
			goto err2;
		}
		if (dp_covered->d_vnode->v_type != VDIR) {
			error = ENOTDIR;
			goto err3;
		}
	}
	mp->m_covered = dp_covered;

	/*
	 * Create a root vnode for this file system.
	 */
	vfscore_vget(mp, 0, &vp);
	if (vp == NULL) {
		error = ENOMEM;
		goto err3;
	}
	vp->v_type = VDIR;
	vp->v_flags = VROOT;
	vp->v_mode = S_IFDIR | S_IRUSR | S_IWUSR | S_IXUSR;

	mp->m_root = dentry_alloc(NULL, vp, "/");
	if (!mp->m_root) {
		error = ENOMEM;
		vput(vp);
		goto err3;
	}
	vput(vp);

	/*
	 * Call a file system specific routine.
	 */
	if ((error = VFS_MOUNT(mp, dev, flags, data)) != 0)
		goto err4;

	if (mp->m_flags & MNT_RDONLY)
		vp->v_mode &=~S_IWUSR;

	/*
	 * Insert to mount list
	 */
	// uk_mutex_lock(&mount_lock);
	uk_list_add_tail(&mp->mnt_list, &mount_list);
	// uk_mutex_unlock(&mount_lock);

	return 0;   /* success */
 err4:
	drele(mp->m_root);
 err3:
	if (dp_covered)
		drele(dp_covered);
 err2:
	free(mp);
 err1:
	if (device)
		device_close(device);

	return -error;
}

void mount_init()
{
	UK_INIT_LIST_HEAD(&mount_list);
}