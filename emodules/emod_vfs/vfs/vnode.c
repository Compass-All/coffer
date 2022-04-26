#include "vnode.h"
#include "mount.h"
#include "../dependency.h"
#include "errno.h"
#include "prex.h"
#include "string.h"

#define __UK_S_BLKSIZE 512

/*
 * Memo:
 *
 * Function   Ref count Lock
 * ---------- --------- ----------
 * vn_lock     *        Lock
 * vn_unlock   *        Unlock
 * vfscore_vget        1        Lock
 * vput       -1        Unlock
 * vref       +1        *
 * vrele      -1        *
 */

#define VNODE_BUCKETS 32		/* size of vnode hash table */

/*
 * vnode table.
 * All active (opened) vnodes are stored on this hash table.
 * They can be accessed by its path name.
 */
static struct uk_list_head vnode_table[VNODE_BUCKETS];


int vn_access(struct vnode *vp, int flags)
{
	int error = 0;

	if ((flags & VEXEC) && (vp->v_mode & 0111) == 0) {
		error = EACCES;
		goto out;
	}
	if ((flags & VREAD) && (vp->v_mode & 0444) == 0) {
		error = EACCES;
		goto out;
	}
	if (flags & VWRITE) {
		if (vp->v_mount->m_flags & MNT_RDONLY) {
			error = EROFS;
			goto out;
		}
		if ((vp->v_mode & 0222) == 0) {
			error = EACCES;
			goto out;
		}
	}
out:
	return error;
}

int vn_stat(struct vnode *vp, struct stat *st)
{
	struct vattr vattr;
	struct vattr *vap;
	mode_t mode;
	int error;

	vap = &vattr;

	memset(st, 0, sizeof(struct stat));

	memset(vap, 0, sizeof(struct vattr));

	error = VOP_GETATTR(vp, vap);
	if (error)
		return error;

	st->st_ino = (ino_t)vap->va_nodeid;
	st->st_size = vap->va_size;
	mode = vap->va_mode;
	switch (vp->v_type) {
	case VREG:
		mode |= S_IFREG;
		break;
	case VDIR:
		mode |= S_IFDIR;
		break;
	case VBLK:
		mode |= S_IFBLK;
		break;
	case VCHR:
		mode |= S_IFCHR;
		break;
	case VLNK:
		mode |= S_IFLNK;
		break;
	case VSOCK:
		mode |= S_IFSOCK;
		break;
	case VFIFO:
		mode |= S_IFIFO;
		break;
	default:
		return EBADF;
	};
	st->st_mode = mode;
	st->st_nlink = vap->va_nlink;
	st->st_blksize = BSIZE;
	st->st_blocks = vap->va_size / __UK_S_BLKSIZE;
	st->st_uid = vap->va_uid;
	st->st_gid = vap->va_gid;
	st->st_dev = vap->va_fsid;
	if (vp->v_type == VCHR || vp->v_type == VBLK)
		st->st_rdev = vap->va_rdev;

	st->st_atim = vap->va_atime;
	st->st_mtim = vap->va_mtime;
	st->st_ctim = vap->va_ctime;

	return 0;
}

void vref(struct vnode *vp)
{
	debug("vref: ref=%d\n", vp->v_refcnt);
	vp->v_refcnt++;
}

static const char *
vn_path(struct vnode *vp)
{
	struct dentry *dp;

	if (uk_list_empty(&vp->v_names) == 1) {
		return (" ");
	}
	dp = uk_list_first_entry(&vp->v_names, struct dentry, d_names_link);
	return (dp->d_path);
}

void vput(struct vnode *vp)
{
	debug("vput: ref=%d %s\n", vp->v_refcnt, vn_path(vp));

	vp->v_refcnt--;
	if (vp->v_refcnt > 0) {
		return;
	}
	uk_list_del(&vp->v_link);

	/*
	 * Deallocate fs specific vnode data
	 */
	if (vp->v_op->vop_inactive)
		VOP_INACTIVE(vp);
	vfs_unbusy(vp->v_mount);
	free(vp);
}

void vrele(struct vnode *vp)
{
	debug("vrele: ref=%d\n", vp->v_refcnt);
	vp->v_refcnt--;
	if (vp->v_refcnt > 0) {
		return;
	}
	uk_list_del(&vp->v_link);

	/*
	 * Deallocate fs specific vnode data
	 */
	VOP_INACTIVE(vp);
	vfs_unbusy(vp->v_mount);
	free(vp);
}

void vn_add_name(struct vnode *vp __unused, struct dentry *dp)
{
	/* TODO: Re-enable this check when preemption and/or smp is
	 * here */
	/* UK_ASSERT(uk_mutex_is_locked(&vp->v_lock)); */
	uk_list_add(&dp->d_names_link, &vp->v_names);
}

void vn_del_name(struct vnode *vp __unused, struct dentry *dp)
{
	/* TODO: Re-enable this check when preemption and/or smp is
	 * here */
	/* UK_ASSERT(uk_mutex_is_locked(&vp->v_lock)); */
	uk_list_del(&dp->d_names_link);
}

/*
 * Get the hash value from the mount point and path name.
 * XXX(hch): replace with a better hash for 64-bit pointers.
 */
static unsigned int vn_hash(struct mount *mp, uint64_t ino)
{
	return (ino ^ (unsigned long)mp) & (VNODE_BUCKETS - 1);
}

/*
 * Returns locked vnode for specified mount point and path.
 * vn_lock() will increment the reference count of vnode.
 *
 * Locking: VNODE_LOCK must be held.
 */
struct vnode *vn_lookup(struct mount *mp, uint64_t ino)
{
	struct vnode *vp;

	show(mp);
	show(ino);

	uk_list_for_each_entry(vp, &vnode_table[vn_hash(mp, ino)], v_link) {
		if (vp->v_mount == mp && vp->v_ino == ino) {
			vp->v_refcnt++;
			show(vp);
			return vp;
		}
	}
	debug("not found\n");
	return NULL;		/* not found */
}

/*
 * Allocate new vnode for specified path.
 * Increment its reference count and lock it.
 * Returns 1 if vnode was found in cache; otherwise returns 0.
 */
int vfscore_vget(struct mount *mp, uint64_t ino, struct vnode **vpp)
{
	struct vnode *vp;
	int error;

	*vpp = NULL;

	show(ino);

	vp = vn_lookup(mp, ino);
	if (vp) {
		*vpp = vp;
		show(vp);
		return 1;
	}

	vp = calloc(1, sizeof(*vp));
	if (!vp) {
		panic("calloc failed\n");
		return 0;
	}

	UK_INIT_LIST_HEAD(&vp->v_names);
	vp->v_ino = ino;
	vp->v_mount = mp;
	vp->v_refcnt = 1;
	vp->v_op = mp->m_op->vfs_vnops;

	show(vp);
	show(vp->v_names);
	show(vp->v_ino);
	show(vp->v_mount);
	show(vp->v_refcnt);
	show(vp->v_op);

	/*
	 * Request to allocate fs specific data for vnode.
	 */
	if ((error = VFS_VGET(mp, vp)) != 0) {
		free(vp);
		return 0;
	}
	vfs_busy(vp->v_mount);

	uk_list_add(&vp->v_link, &vnode_table[vn_hash(mp, ino)]);

	*vpp = vp;

	return 0;
}

int vfscore_vop_nullop()
{
	return 0;
}

int vfscore_vop_einval()
{
	return EINVAL;
}

int vfscore_vop_eperm()
{
	return EPERM;
}

int vfscore_vop_erofs()
{
	return EROFS;
}

void vnode_init(void)
{
	int i;

	for (i = 0; i < VNODE_BUCKETS; i++)
		UK_INIT_LIST_HEAD(&vnode_table[i]);
}