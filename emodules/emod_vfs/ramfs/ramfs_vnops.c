#include "ramfs.h"
#include "../vfs/vnode.h"
#include "../dependency.h"
#include "string.h"
#include "errno.h"
#include <memory/memory.h>
#include "../vfs/fs.h"
#include "../vfs/limits.h"
#include "../vfs/dirent.h"

#define true	1
#define false	0

static uint64_t inode_count = 1;

static void set_times_to_now(
	struct timespec *time1, 
	struct timespec *time2,
	struct timespec *time3
)
{
	struct timespec now = {0, 0};

	/* TODO: implement the real clock_gettime */
	/* clock_gettime(CLOCK_REALTIME, &now); */
	if (time1)
		memcpy(time1, &now, sizeof(struct timespec));
	if (time2)
		memcpy(time2, &now, sizeof(struct timespec));
	if (time3)
		memcpy(time3, &now, sizeof(struct timespec));
}

struct ramfs_node *ramfs_allocate_node(
	const char *name,
	int type
)
{
	struct ramfs_node *np;

	np = kmalloc(sizeof(struct ramfs_node));
	if (np == NULL)
		return NULL;
	memset(np, 0, sizeof(struct ramfs_node));

	np->rn_namelen = strlen(name);
	np->rn_name = (char *) kmalloc(np->rn_namelen + 1);
	if (np->rn_name == NULL) {
		free(np);
		return NULL;
	}
	strlcpy(np->rn_name, name, np->rn_namelen + 1);
	np->rn_type = type;

	if (type == VDIR)
		np->rn_mode = S_IFDIR|0777;
	else if (type == VLNK)
		np->rn_mode = S_IFLNK|0777;
	else
		np->rn_mode = S_IFREG|0777;

	set_times_to_now(
		&(np->rn_ctime),
		&(np->rn_atime),
		&(np->rn_mtime)
	);
	np->rn_owns_buf = true;

	return np;
}

void ramfs_free_node(struct ramfs_node *np)
{
	if (np->rn_buf != NULL && np->rn_owns_buf)
		free(np->rn_buf);

	free(np->rn_name);
	free(np);
}

static struct ramfs_node *ramfs_add_node(
	struct ramfs_node *dnp,
	char *name,
	int type
)
{
	struct ramfs_node *np, *prev;

	np = ramfs_allocate_node(name, type);
	if (np == NULL)
		return NULL;

	/* Link to the directory list */
	if (dnp->rn_child == NULL) {
		dnp->rn_child = np;
	} else {
		prev = dnp->rn_child;
		while (prev->rn_next != NULL)
			prev = prev->rn_next;
		prev->rn_next = np;
	}

	set_times_to_now(
		&(dnp->rn_mtime),
		&(dnp->rn_ctime),
		NULL
	);

	return np;
}

static int ramfs_remove_node(
	struct ramfs_node *dnp,
	struct ramfs_node *np
)
{
	struct ramfs_node *prev;

	if (dnp->rn_child == NULL)
		return EBUSY;

	/* Unlink from the directory list */
	if (dnp->rn_child == np) {
		dnp->rn_child = np->rn_next;
	} else {
		for (prev = dnp->rn_child; prev->rn_next != np;
			 prev = prev->rn_next) {
			if (prev->rn_next == NULL) {
				return ENOENT;
			}
		}
		prev->rn_next = np->rn_next;
	}
	ramfs_free_node(np);

	set_times_to_now(
		&(dnp->rn_mtime),
		&(dnp->rn_ctime),
		NULL
	);

	return 0;
}

static int ramfs_rename_node(
	struct ramfs_node *np,
	char *name
)
{
	size_t len;
	char *tmp;

	len = strlen(name);
	if (len > NAME_MAX)
		return ENAMETOOLONG;

	if (len <= np->rn_namelen) {
		/* Reuse current name buffer */
		strlcpy(np->rn_name, name, np->rn_namelen + 1);
	} else {
		/* Expand name buffer */
		tmp = (char *) kmalloc(len + 1);
		if (tmp == NULL)
			return ENOMEM;
		strlcpy(tmp, name, len + 1);
		free(np->rn_name);
		np->rn_name = tmp;
	}
	np->rn_namelen = len;
	set_times_to_now(&(np->rn_ctime), NULL, NULL);
	return 0;
}

static int ramfs_lookup(
	struct vnode *dvp,
	char *name,
	struct vnode **vpp
)
{
	struct ramfs_node *np, *dnp;
	struct vnode *vp;
	size_t len;
	int found;

	*vpp = NULL;

	if (*name == '\0')
		return ENOENT;

	len = strlen(name);
	dnp = dvp->v_data;
	found = 0;
	for (np = dnp->rn_child; np != NULL; np = np->rn_next) {
		if (np->rn_namelen == len &&
			memcmp(name, np->rn_name, len) == 0) {
			found = 1;
			break;
		}
	}
	if (found == 0) {
		return ENOENT;
	}
	if (vfscore_vget(dvp->v_mount, inode_count++, &vp)) {
		/* found in cache */
		*vpp = vp;
		return 0;
	}
	if (!vp) {
		return ENOMEM;
	}
	vp->v_data = np;
	vp->v_mode = UK_ALLPERMS;
	vp->v_type = np->rn_type;
	vp->v_size = np->rn_size;

	*vpp = vp;

	return 0;
}


static int ramfs_mkdir(
	struct vnode *dvp,
	char *name,
	mode_t mode
)
{
	struct ramfs_node *np;

	debug("mkdir %s\n", name);
	if (strlen(name) > NAME_MAX)
		return ENAMETOOLONG;

	if (!S_ISDIR(mode))
		return EINVAL;

	np = ramfs_add_node(dvp->v_data, name, VDIR);
	if (np == NULL)
		return ENOMEM;
	np->rn_size = 0;

	return 0;
}

static int ramfs_symlink(
	struct vnode *dvp,
	char *name,
	char *link
)
{
	struct ramfs_node *np;
	size_t len;

	if (strlen(name) > NAME_MAX)
		return ENAMETOOLONG;

	np = ramfs_add_node(dvp->v_data, name, VLNK);

	if (np == NULL)
		return ENOMEM;
	// Save the link target without the final null, as readlink() wants it.
	len = strlen(link);

	np->rn_buf = strndup(link, len);
	np->rn_bufsize = np->rn_size = len;

	return 0;
}

static int ramfs_readlink(struct vnode *vp, struct uio *uio)
{
	struct ramfs_node *np = vp->v_data;
	size_t len;

	if (vp->v_type != VLNK)
		return EINVAL;
	if (uio->uio_offset < 0)
		return EINVAL;
	if (uio->uio_resid == 0)
		return 0;
	if (uio->uio_offset >= (off_t) vp->v_size)
		return 0;
	if (vp->v_size - uio->uio_offset < uio->uio_resid)
		len = vp->v_size - uio->uio_offset;
	else
		len = uio->uio_resid;

	set_times_to_now(&(np->rn_atime), NULL, NULL);
	return vfscore_uiomove(
		np->rn_buf + uio->uio_offset,
		len,
		uio
	);
}

/* Remove a directory */
static int ramfs_rmdir(
	struct vnode *dvp,
	struct vnode *vp,
	char *name __unused
)
{
	return ramfs_remove_node(dvp->v_data, vp->v_data);
}

/* Remove a file */
static int ramfs_remove(
	struct vnode *dvp,
	struct vnode *vp,
	char *name
)
{
	debug("remove %s in %s\n", name,
		RAMFS_NODE(dvp)->rn_name);
	return ramfs_remove_node(dvp->v_data, vp->v_data);
}

/* Truncate file */
static int ramfs_truncate(struct vnode *vp, off_t length)
{
	struct ramfs_node *np;
	void *new_buf;
	size_t new_size;

	debug("truncate %s length=%lld\n",
		RAMFS_NODE(vp)->rn_name,
		(long long) length);
	np = vp->v_data;

	if (length == 0) {
		if (np->rn_buf != NULL) {
			if (np->rn_owns_buf)
				free(np->rn_buf);
			np->rn_buf = NULL;
			np->rn_bufsize = 0;
		}
	} else if ((size_t) length > np->rn_bufsize) {
		/* TODO: this could use a page level allocator */
		new_size = PARTITION_UP(length) + 2 * PARTITION_SIZE;
		new_buf = kmalloc(new_size);
		if (!new_buf)
			return EIO;
		if (np->rn_size != 0) {
			memcpy(new_buf, np->rn_buf, vp->v_size);
			if (np->rn_owns_buf)
				free(np->rn_buf);
		}
		np->rn_buf = (char *) new_buf;
		np->rn_bufsize = new_size;
		np->rn_owns_buf = true;
	}
	np->rn_size = length;
	vp->v_size = length;
	set_times_to_now(
		&(np->rn_mtime),
		&(np->rn_ctime),
		NULL
	);
	return 0;
}

/*
 * Create empty file.
 */
static int ramfs_create(
	struct vnode *dvp,
	char *name,
	mode_t mode
)
{
	struct ramfs_node *np;

	if (strlen(name) > NAME_MAX)
		return ENAMETOOLONG;

	debug("create %s in %s\n",
		name,
		RAMFS_NODE(dvp)->rn_name);
	if (!S_ISREG(mode))
		return EINVAL;

	np = ramfs_add_node(dvp->v_data, name, VREG);
	if (np == NULL)
		return ENOMEM;
	return 0;
}

static int ramfs_read(
	struct vnode *vp,
	struct vfscore_file *fp __unused,
	struct uio *uio,
	int ioflag __unused
)
{
	struct ramfs_node *np =  vp->v_data;
	size_t len;

	if (vp->v_type == VDIR)
		return EISDIR;
	if (vp->v_type != VREG)
		return EINVAL;
	if (uio->uio_offset < 0)
		return EINVAL;
	if (uio->uio_resid == 0)
		return 0;

	if (uio->uio_offset >= (off_t) vp->v_size)
		return 0;

	if (vp->v_size - uio->uio_offset < uio->uio_resid)
		len = vp->v_size - uio->uio_offset;
	else
		len = uio->uio_resid;

	set_times_to_now(&(np->rn_atime), NULL, NULL);

	return vfscore_uiomove(
		np->rn_buf + uio->uio_offset,
		len,
		uio
	);
}

int ramfs_set_file_data(
	struct vnode *vp,
	const void *data,
	size_t size
)
{
	struct ramfs_node *np =  vp->v_data;

	if (vp->v_type == VDIR)
		return EISDIR;
	if (vp->v_type != VREG)
		return EINVAL;
	if (np->rn_buf)
		return EINVAL;

	np->rn_buf = (char *) data;
	np->rn_bufsize = size;
	np->rn_size = size;
	vp->v_size = size;
	np->rn_owns_buf = false;

	return 0;
}

static int ramfs_write(
	struct vnode *vp,
	struct uio *uio,
	int ioflag
)
{
	// start_timer();

	struct ramfs_node *np =  vp->v_data;

	if (vp->v_type == VDIR)
		return EISDIR;
	if (vp->v_type != VREG)
		return EINVAL;
	if (uio->uio_offset < 0)
		return EINVAL;
	if (uio->uio_offset >= LONG_MAX)
		return EFBIG;
	if (uio->uio_resid == 0)
		return 0;

	if (ioflag & IO_APPEND)
		uio->uio_offset = np->rn_size;

	if ((size_t) uio->uio_offset + uio->uio_resid > (size_t) vp->v_size) {
		/* Expand the file size before writing to it */
		off_t end_pos = uio->uio_offset + uio->uio_resid;

		if (end_pos > (off_t) np->rn_bufsize) {
			show(end_pos);
			show(np->rn_bufsize);

			size_t new_size;
			// to do: this could use a page level allocator
			if (np->rn_bufsize) {
				info("Enlarging file with 100 extra partitions\n");
				new_size = PARTITION_UP(end_pos) + 100 * PARTITION_SIZE;
			} else {
				// PARTITION_UP causes OOM for yolov3 bench
				new_size = PAGE_UP(end_pos);
			}

			// new_size = PARTITION_UP(end_pos) + 100 * PARTITION_SIZE;

			// call_timer();
			void *new_buf = calloc(1, new_size);
			// call_timer();

			if (!new_buf)
				return EIO;
			if (np->rn_size != 0) {
				// printf("buf size: 0x%lx\n", vp->v_size);
				// check this: takes long time
				// call_timer();
				memcpy(new_buf, np->rn_buf, vp->v_size);
				// call_timer();
				if (np->rn_owns_buf) {
					free(np->rn_buf);
					// call_timer();
				}
			}
			np->rn_buf = (char *) new_buf;
			np->rn_bufsize = new_size;
		}
		np->rn_size = end_pos;
		vp->v_size = end_pos;
		np->rn_owns_buf = true;
	}

	// call_timer();

	set_times_to_now(
		&(np->rn_mtime),
		&(np->rn_ctime),
		NULL
	);
	int ret = vfscore_uiomove(
		np->rn_buf + uio->uio_offset,
		uio->uio_resid,
		uio
	);

	// call_timer();
	// end_timer();

	return ret;
}

static int ramfs_rename(
	struct vnode *dvp1,
	struct vnode *vp1,
	char *name1 __unused,
	struct vnode *dvp2,
	struct vnode *vp2,
	char *name2
)
{
	struct ramfs_node *np, *old_np;
	int error;

	if (vp2) {
		/* Remove destination file, first */
		error = ramfs_remove_node(dvp2->v_data, vp2->v_data);
		if (error)
			return error;
	}
	/* Same directory ? */
	if (dvp1 == dvp2) {
		/* Change the name of existing file */
		error = ramfs_rename_node(vp1->v_data, name2);
		if (error)
			return error;
	} else {
		/* Create new file or directory */
		old_np = vp1->v_data;
		np = ramfs_add_node(dvp2->v_data, name2, old_np->rn_type);
		if (np == NULL)
			return ENOMEM;

		if (old_np->rn_buf) {
			/* Copy file data */
			np->rn_buf = old_np->rn_buf;
			np->rn_size = old_np->rn_size;
			np->rn_bufsize = old_np->rn_bufsize;
			old_np->rn_buf = NULL;
		}
		/* Remove source file */
		ramfs_remove_node(dvp1->v_data, vp1->v_data);
	}
	return 0;
}

/*
 * @vp: vnode of the directory.
 */
static int ramfs_readdir(
	struct vnode *vp,
	struct vfscore_file *fp,
	struct dirent *dir
)
{
	struct ramfs_node *np, *dnp;
	int i;

	set_times_to_now(
		&(((struct ramfs_node *) vp->v_data)->rn_atime),
		NULL,
		NULL
	);

	if (fp->f_offset == 0) {
		dir->d_type = DT_DIR;
		strlcpy((char *) &dir->d_name, ".", sizeof(dir->d_name));
	} else if (fp->f_offset == 1) {
		dir->d_type = DT_DIR;
		strlcpy((char *) &dir->d_name, "..", sizeof(dir->d_name));
	} else {
		dnp = vp->v_data;
		np = dnp->rn_child;
		if (np == NULL) {
			return ENOENT;
		}

		for (i = 0; i != (fp->f_offset - 2); i++) {
			np = np->rn_next;
			if (np == NULL) {
				return ENOENT;
			}
		}
		if (np->rn_type == VDIR)
			dir->d_type = DT_DIR;
		else if (np->rn_type == VLNK)
			dir->d_type = DT_LNK;
		else
			dir->d_type = DT_REG;
		strlcpy((char *) &dir->d_name, np->rn_name,
				sizeof(dir->d_name));
	}
	dir->d_fileno = fp->f_offset;
//	dir->d_namelen = strlen(dir->d_name);

	fp->f_offset++;

	return 0;
}

static int ramfs_getattr(
	struct vnode *vnode,
	struct vattr *attr
)
{
	struct ramfs_node *np = vnode->v_data;

	attr->va_nodeid = vnode->v_ino;
	attr->va_size = vnode->v_size;

	attr->va_type = np->rn_type;

	memcpy(&(attr->va_atime), &(np->rn_atime), sizeof(struct timespec));
	memcpy(&(attr->va_ctime), &(np->rn_ctime), sizeof(struct timespec));
	memcpy(&(attr->va_mtime), &(np->rn_mtime), sizeof(struct timespec));

	attr->va_mode = np->rn_mode;

	return 0;
}

static int ramfs_setattr(
	struct vnode *vnode,
	struct vattr *attr
)
{
	struct ramfs_node *np = vnode->v_data;

	if (attr->va_mask & AT_ATIME) {
		memcpy(&(np->rn_atime), &(attr->va_atime),
		       sizeof(struct timespec));
	}

	if (attr->va_mask & AT_CTIME) {
		memcpy(&(np->rn_ctime), &(attr->va_ctime),
		       sizeof(struct timespec));
	}

	if (attr->va_mask & AT_MTIME) {
		memcpy(&(np->rn_mtime), &(attr->va_mtime),
		       sizeof(struct timespec));
	}

	if (attr->va_mask & AT_MODE)
		np->rn_mode = attr->va_mode;

	return 0;
}

#define ramfs_open      ((vnop_open_t)vfscore_vop_nullop)
#define ramfs_close     ((vnop_close_t)vfscore_vop_nullop)
#define ramfs_seek      ((vnop_seek_t)vfscore_vop_nullop)
#define ramfs_ioctl     ((vnop_ioctl_t)vfscore_vop_einval)
#define ramfs_fsync     ((vnop_fsync_t)vfscore_vop_nullop)
#define ramfs_inactive  ((vnop_inactive_t)vfscore_vop_nullop)
#define ramfs_link      ((vnop_link_t)vfscore_vop_eperm)
#define ramfs_fallocate ((vnop_fallocate_t)vfscore_vop_nullop)

struct vnops ramfs_vnops;

void ramfs_vnops_init()
{
		ramfs_vnops.vop_open 		= ramfs_open;
		ramfs_vnops.vop_close 		= ramfs_close;
		ramfs_vnops.vop_read 		= ramfs_read;
		ramfs_vnops.vop_write 		= ramfs_write;
		ramfs_vnops.vop_seek 		= ramfs_seek;
		ramfs_vnops.vop_ioctl 		= ramfs_ioctl;
		ramfs_vnops.vop_fsync 		= ramfs_fsync;
		ramfs_vnops.vop_readdir 	= ramfs_readdir;
		ramfs_vnops.vop_lookup 		= ramfs_lookup;
		ramfs_vnops.vop_create 		= ramfs_create;
		ramfs_vnops.vop_remove 		= ramfs_remove;
		ramfs_vnops.vop_rename 		= ramfs_rename;
		ramfs_vnops.vop_mkdir 		= ramfs_mkdir;
		ramfs_vnops.vop_rmdir 		= ramfs_rmdir;
		ramfs_vnops.vop_getattr 	= ramfs_getattr;
		ramfs_vnops.vop_setattr 	= ramfs_setattr;
		ramfs_vnops.vop_inactive 	= ramfs_inactive;
		ramfs_vnops.vop_truncate 	= ramfs_truncate;
		ramfs_vnops.vop_link 		= ramfs_link;
		ramfs_vnops.vop_cache 		= (vnop_cache_t) NULL;
		ramfs_vnops.vop_fallocate 	= ramfs_fallocate;
		ramfs_vnops.vop_readlink 	= ramfs_readlink;
		ramfs_vnops.vop_symlink 	= ramfs_symlink;
}