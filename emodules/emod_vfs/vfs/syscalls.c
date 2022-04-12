#include "syscalls.h"
#include "file.h"
#include "../dependency.h"
#include "fs.h"
#include "fops.h"
#include "lookup.h"
#include "vnode.h"
#include <errno.h>

static int
open_no_follow_chk(char *path)
{
	// todo
	return 0;
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

static void memcpy(void *dest, const void *src, size_t count)
{
	char *temp1	  = dest;
	const char *temp2 = src;

	while (count > 0) {
		*temp1++ = *temp2++;
		count--;
	}
}

int
sys_read(struct vfscore_file *fp, const struct iovec *iov, size_t niov,
		off_t offset, size_t *count)
{
	int error = 0;
	struct iovec *copy_iov;
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
	copy_iov = calloc(sizeof(struct iovec), niov);
	if (!copy_iov)
		return ENOMEM;
	memcpy(copy_iov, iov, sizeof(struct iovec)*niov);

	uio.uio_iov = copy_iov;
	uio.uio_iovcnt = niov;
	uio.uio_offset = offset;
	uio.uio_resid = bytes;
	uio.uio_rw = UIO_READ;
	error = vfs_read(fp, &uio, (offset == -1) ? 0 : FOF_OFFSET);
	*count = bytes - uio.uio_resid;

	free(copy_iov);
	return error;
}