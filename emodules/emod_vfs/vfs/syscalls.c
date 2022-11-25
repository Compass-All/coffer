#include "syscalls.h"
#include "file.h"
#include "../dependency.h"
#include "fs.h"
#include "fops.h"
#include "lookup.h"
#include "vnode.h"
#include <string.h>
#include "errno.h"
#include "fcntl.h"
#include "ioctl.h"

static int
open_no_follow_chk(char *path)
{
	int           error;
	struct dentry *ddp;
	char          *name;
	struct dentry *dp;
	struct vnode  *vp;

	ddp = NULL;
	dp  = NULL;
	vp  = NULL;

	error = lookup(path, &ddp, &name);
	if (error) {
		return (error);
	}

	error = namei_last_nofollow(path, ddp, &dp);
	if (error) {
		goto out;
	}

	vp = dp->d_vnode;
	if (vp->v_type == VLNK) {
		error = ELOOP;
		goto out;
	}

	error = 0;
out:
	if (dp != NULL) {
		drele(dp);
	}

	if (ddp != NULL) {
		drele(ddp);
	}

	return (error);
}

int sys_open(char *path, int flags, mode_t mode, struct vfscore_file **fpp)
{
	struct vfscore_file *fp;
	struct dentry *dp, *ddp;
	struct vnode *vp;
	char *filename;
	int error;

	debug("sys_open: path=%s flags=%x mode=%x\n",
				path, flags, mode);
	flags = vfscore_fflags(flags);
	if (flags & O_CREAT) {
		error = namei(path, &dp);
		if (error == ENOENT) {
			if ((error = lookup(path, &ddp, &filename)) != 0)
				return error;

			if ((error = vn_access(ddp->d_vnode, VWRITE)) != 0) {
				drele(ddp);
				return error;
			}
			mode &= ~S_IFMT;
			mode |= S_IFREG;
			error = VOP_CREATE(ddp->d_vnode, filename, mode);
			drele(ddp);

			if (error)
				return error;
			if ((error = namei(path, &dp)) != 0)
				return error;

			vp = dp->d_vnode;
			flags &= ~O_TRUNC;
		} else if (error) {
			return error;
		} else {
			if (flags & O_EXCL) {
				error = EEXIST;
				goto out_drele;
			}
		}

		vp = dp->d_vnode;
		flags &= ~O_CREAT;
	} else {
		if (flags & O_NOFOLLOW) {
			error = open_no_follow_chk(path);
			if (error != 0)
				return error;
		}
		error = namei(path, &dp);
		if (error)
			return error;

		vp = dp->d_vnode;

		if (flags & UK_FWRITE || flags & O_TRUNC) {
			error = vn_access(vp, VWRITE);
			if (error)
				goto out_drele;

			error = EISDIR;
			if (vp->v_type == VDIR)
				goto out_drele;
		}
		if (flags & O_DIRECTORY) {
			if (vp->v_type != VDIR) {
				error = ENOTDIR;
				goto out_drele;
			}
		}
	}

	fp = calloc(sizeof(struct vfscore_file), 1);
	if (!fp) {
		error = ENOMEM;
		goto out_drele;
	}

	fhold(fp);
	fp->f_flags = flags;

	fp->f_dentry = dp;

	if (flags & O_TRUNC) {
		error = EINVAL;
		if (!(flags & UK_FWRITE) || vp->v_type == VDIR)
			goto out_fp_free_unlock;

		error = VOP_TRUNCATE(vp, 0);
		if (error)
			goto out_fp_free_unlock;
	}

	error = VOP_OPEN(vp, fp);
	if (error)
		goto out_fp_free_unlock;

	*fpp = fp;
	return 0;

out_fp_free_unlock:
	free(fp);

out_drele:
	if (dp)
		drele(dp);
	return error;
}

int sys_close(struct vfscore_file *fp __unused)
{
	return 0;
}

int sys_read(struct vfscore_file *fp, const struct iovec *iov, size_t niov,
		off_t offset, size_t *count)
{
	int error = 0;
	if ((fp->f_flags & UK_FREAD) == 0)
		return EBADF;

	size_t bytes = 0;
	const struct iovec *iovp = iov;

	for (unsigned i = 0; i < niov; i++) {
		if (iovp->iov_len > IOSIZE_MAX - bytes) {
			return EINVAL;
		}
		bytes += iovp->iov_len;
		iovp++;
	}

	if (bytes == 0) {
		*count = 0;
		return 0;
	}

	struct uio uio;
	/* TODO: is it necessary to copy iov within Unikraft?
	 * OSv did this, mentioning this reason:
	 *
	 * "Unfortunately, the current implementation of fp->read
	 *  zeros the iov_len fields when it reads from disk, so we
	 *  have to copy iov. "
	 */
	struct iovec copy_iov[niov];
	memcpy(copy_iov, iov, sizeof(struct iovec)*niov);

	uio.uio_iov = copy_iov;
	uio.uio_iovcnt = niov;
	uio.uio_offset = offset;
	uio.uio_resid = bytes;
	uio.uio_rw = UIO_READ;
	error = vfs_read(fp, &uio, (offset == -1) ? 0 : FOF_OFFSET);
	*count = bytes - uio.uio_resid;

	// free(copy_iov);
	return error;
}


int sys_write(struct vfscore_file *fp, const struct iovec *iov, size_t niov,
		off_t offset, size_t *count)
{
	int error = 0;
	if ((fp->f_flags & UK_FWRITE) == 0)
		return EBADF;

	size_t bytes = 0;
	const struct iovec *iovp = iov;
	for (unsigned i = 0; i < niov; i++) {
		if (iovp->iov_len > IOSIZE_MAX - bytes) {
			return EINVAL;
		}
		bytes += iovp->iov_len;
		iovp++;
	}

	if (bytes == 0) {
		*count = 0;
		return 0;
	}

	struct uio uio;

	/* TODO: same note as in sys_read. Original comment:
	 *
	 * "Unfortunately, the current implementation of fp->write zeros the
	 *  iov_len fields when it writes to disk, so we have to copy iov.
	 */
	/* std::vector<iovec> copy_iov(iov, iov + niov); */
	struct iovec copy_iov[niov];
	memcpy(copy_iov, iov, sizeof(struct iovec)*niov);

	uio.uio_iov = copy_iov;
	uio.uio_iovcnt = niov;
	uio.uio_offset = offset;
	uio.uio_resid = bytes;
	uio.uio_rw = UIO_WRITE;
	// check this: takes long time
	error = vfs_write(fp, &uio, (offset == -1) ? 0 : FOF_OFFSET);
	*count = bytes - uio.uio_resid;

	// free(copy_iov);
	return error;
}

int sys_lseek(struct vfscore_file *fp, off_t off, int type, off_t *origin)
{
	struct vnode *vp;

	debug("sys_seek: fp=%p off=%ud type=%d\n",
			fp, (unsigned int)off, type);

	if (!fp->f_dentry) {
	    // Linux doesn't implement lseek() on pipes, sockets, or ttys.
	    // In OSV, we only implement lseek() on regular files, backed by vnode
	    return ESPIPE;
	}

	vp = fp->f_dentry->d_vnode;
	int error = EINVAL;
	switch (type) {
	case SEEK_CUR:
		off = fp->f_offset + off;
		break;
	case SEEK_END:
		off = vp->v_size + off;
		break;
	}
	if (off >= 0) {
		error = VOP_SEEK(vp, fp, fp->f_offset, off);
		if (!error) {
			*origin      = off;
			fp->f_offset = off;
		}
	}
	return error;
}

int sys_ioctl(struct vfscore_file *fp, unsigned long request, void *buf)
{
	int error = 0;

	debug("sys_ioctl: fp=%p request=%lux\n", fp, request);

	if ((fp->f_flags & (UK_FREAD | UK_FWRITE)) == 0)
		return EBADF;

	switch (request) {
	case FIOCLEX:
		fp->f_flags |= O_CLOEXEC;
		break;
	case FIONCLEX:
		fp->f_flags &= ~O_CLOEXEC;
		break;
	default:
		error = vfs_ioctl(fp, request, buf);
		break;
	}

	debug("sys_ioctl: comp error=%d\n", error);
	return error;
}

int sys_fstat(struct vfscore_file *fp, struct stat *st)
{
	int error = 0;

	debug("sys_fstat: fp=%p\n", fp);

	error = vfs_stat(fp, st);

	return error;
}


int sys_stat(char *path, struct stat *st)
{
	struct dentry *dp;
	int error;

	debug("sys_stat: path=%s\n", path);

	error = namei(path, &dp);
	if (error)
		return error;
	error = vn_stat(dp->d_vnode, st);
	drele(dp);
	return error;
}

int sys_lstat(char *path, struct stat *st)
{
	int           error;
	struct dentry *ddp;
	char          *name;
	struct dentry *dp;

	debug("sys_lstat: path=%s\n", path);

	error = lookup(path, &ddp, &name);
	if (error) {
		return (error);
	}

	error = namei_last_nofollow(path, ddp, &dp);
	if (error) {
		drele(ddp);
		return error;
	}

	error = vn_stat(dp->d_vnode, st);
	drele(dp);
	drele(ddp);
	return error;
}

int sys_mkdir(char *path, mode_t mode)
{
	char *name;
	struct dentry *dp, *ddp;
	int error;

	debug("sys_mkdir: path=%s mode=%d\n", path, mode);

	error = namei(path, &dp);
	if (!error) {
		/* File already exists */
		drele(dp);
		return EEXIST;
	}

	if ((error = lookup(path, &ddp, &name)) != 0) {
		/* Directory already exists */
		return error;
	}

	if ((error = vn_access(ddp->d_vnode, VWRITE)) != 0)
		goto out;
	mode &= ~S_IFMT;
	mode |= S_IFDIR;

	error = VOP_MKDIR(ddp->d_vnode, name, mode);
 out:
	drele(ddp);
	return error;
}

int sys_readdir(struct vfscore_file *fp, struct dirent *dir)
{
	struct vnode *dvp;
	int error;

	debug("sys_readdir: fp=%p\n", fp);

	if (!fp->f_dentry)
		return ENOTDIR;

	dvp = fp->f_dentry->d_vnode;
	if (dvp->v_type != VDIR) {
		return ENOTDIR;
	}
	error = VOP_READDIR(dvp, fp, dir);
	debug("sys_readdir: error=%d path=%s\n",
				error, dir->d_name);
	return error;
}

int sys_fsync(struct vfscore_file *fp)
{
	struct vnode *vp;
	int error;

	debug("sys_fsync: fp=%p\n", fp);

	if (!fp->f_dentry)
		return EINVAL;

	vp = fp->f_dentry->d_vnode;
	error = VOP_FSYNC(vp, fp);
	return error;
}

int sys_unlink(char *path)
{
	char *name;
	struct dentry *dp, *ddp;
	struct vnode *vp;
	int error;

	debug("sys_unlink: path=%s\n", path);

	ddp   = NULL;
	dp    = NULL;
	vp    = NULL;

	error = lookup(path, &ddp, &name);
	if (error != 0) {
		return (error);
	}

	debug("CP1\n");

	error = namei_last_nofollow(path, ddp, &dp);
	if (error != 0) {
		goto out;
	}

	debug("CP2\n");

	vp = dp->d_vnode;
	if (vp->v_type == VDIR) {
	    // Posix specifies that we should return EPERM here, but Linux
	    // actually returns EISDIR.
		error = EISDIR;
		goto out;
	}
	if (vp->v_flags & VROOT) {
		error = EBUSY;
		goto out;
	}

	if ((error = vn_access(ddp->d_vnode, VWRITE)) != 0) {
	    goto out;
	}
	error = VOP_REMOVE(ddp->d_vnode, vp, name);

	debug("CP3\n");

	dentry_remove(dp);
	drele(ddp);
	drele(dp);

	debug("CP4\n");

	return error;
 out:

	if (dp != NULL) {
		drele(dp);
	}

	if (ddp != NULL) {
		drele(ddp);
	}
	return error;
}

int sys_ftruncate(struct vfscore_file *fp, off_t length)
{
	struct vnode *vp;
	int error;

	if (!fp->f_dentry)
		return EBADF;

	vp = fp->f_dentry->d_vnode;
	error = VOP_TRUNCATE(vp, length);

	return error;
}