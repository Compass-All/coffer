#include "fops.h"
#include "../dependency.h"
#include "dentry.h"
#include "vnode.h"
#include "fcntl.h"

int vfs_read(struct vfscore_file *fp, struct uio *uio, int flags)
{
	struct vnode *vp = fp->f_dentry->d_vnode;
	int error;
	size_t count;
	ssize_t bytes;

	bytes = uio->uio_resid;

	if ((flags & FOF_OFFSET) == 0)
		uio->uio_offset = fp->f_offset;

	error = VOP_READ(vp, fp, uio, 0);
	if (!error) {
		count = bytes - uio->uio_resid;
		if (((flags & FOF_OFFSET) == 0) &&
		    !(fp->f_vfs_flags & UK_VFSCORE_NOPOS))
			fp->f_offset += count;
	}

	return error;
}

int vfs_write(struct vfscore_file *fp, struct uio *uio, int flags)
{
	struct vnode *vp = fp->f_dentry->d_vnode;
	int ioflags = 0;
	int error;
	size_t count;
	ssize_t bytes;

	bytes = uio->uio_resid;

	if (fp->f_flags & O_APPEND)
		ioflags |= IO_APPEND;
	if (fp->f_flags & (O_DSYNC|O_SYNC))
		ioflags |= IO_SYNC;

	if ((flags & FOF_OFFSET) == 0)
		uio->uio_offset = fp->f_offset;

	error = VOP_WRITE(vp, uio, ioflags);
	if (!error) {
		count = bytes - uio->uio_resid;
		if (!(flags & FOF_OFFSET) &&
		    !(fp->f_vfs_flags & UK_VFSCORE_NOPOS))
			fp->f_offset += count;
	}

	return error;
}

int vfs_ioctl(struct vfscore_file *fp, unsigned long com, void *data)
{
	struct vnode *vp = fp->f_dentry->d_vnode;
	int error;

	error = VOP_IOCTL(vp, fp, com, data);

	return error;
}

int vfs_stat(struct vfscore_file *fp, struct stat *st)
{
	struct vnode *vp = fp->f_dentry->d_vnode;
	int error;

	error = vn_stat(vp, st);

	return error;
}

int vfs_close(struct vfscore_file *fp)
{
	struct vnode *vp = fp->f_dentry->d_vnode;
	int error;

	error = VOP_CLOSE(vp, fp);

	if (error)
		return error;

	/* Release the dentry */
	drele(fp->f_dentry);
	fp->f_dentry = NULL;

	return 0;
}