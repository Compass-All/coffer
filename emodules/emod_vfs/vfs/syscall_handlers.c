#include "syscall_handlers.h"
#include "syscalls.h"
#include <emodules/emod_vfs/iovec.h>
#include "task.h"
#include "file.h"
#include "fcntl.h"
#include "fd.h"
#include "vnode.h"
#include "errno.h"
#include <string.h>
#include "stat.h"
#include "lookup.h"
#include "../dependency.h"

// readv, openat, close, lseek, read, writev, ioctl, fstat

struct task *main_task; /* we only have a single process */

int	syscall_handler_open(const char *pathname, int flags, mode_t mode)
{
	struct task *t = main_task;
	char path[PATH_MAX];
	struct vfscore_file *fp;
	int fd, error;
	int acc;

	acc = 0;
	switch (flags & O_ACCMODE)
	{
	case O_RDONLY:
		acc = VREAD;
		break;
	
	case O_WRONLY:
		acc = VWRITE;
		break;

	case O_RDWR:
		acc = VREAD | VWRITE;
		break;
	}

	error = task_conv(t, pathname, acc, path);
	if (error)
		goto out_error;

	error = sys_open(path, flags, mode, &fp);
	if (error)
		goto out_error;

	error = fdalloc(fp, &fd);
	if (error)
		goto out_fput;
	fdrop(fp);

	show(fd);

	return fd;

out_fput:
	fdrop(fp);

out_error:
	return -error;
}

int syscall_handler_openat(int dirfd, const char *pathname, int flags, int mode)
{
	if (pathname[0] == '/' || dirfd == AT_FDCWD) {
		return syscall_handler_open(pathname, flags, (mode_t)mode);
	}

	struct vfscore_file *fp;
	int error = fget(dirfd, &fp);
	if (error) {
		errno = error;
		return -1;
	}

	// struct vnode *vp = fp->f_dentry->d_vnode;

	char p[PATH_MAX];

	/* build absolute path */
	strlcpy(p, fp->f_dentry->d_mount->m_path, PATH_MAX);
	strlcat(p, fp->f_dentry->d_path, PATH_MAX);
	strlcat(p, "/", PATH_MAX);
	strlcat(p, pathname, PATH_MAX);

	error = syscall_handler_open(p, flags, (mode_t)mode);

	fdrop(fp);

	return -error;
}

static int fdclose(int fd)
{
	struct vfscore_file *fp;
	int error;

	fp = vfscore_get_file(fd);
	if (!fp)
		return EBADF;

	error = vfscore_put_fd(fd);
	if (!error)
		fdrop(fp);

	return error;
}

int syscall_handler_close(int fd)
{
	int error;

	error = fdclose(fd);
	if (error)
		goto out_error;

	return 0;

out_error:
	return -error;
}

int syscall_handler_lseek(int fd, off_t offset, int whence)
{
	struct vfscore_file *fp;
	off_t org;
	int error;

	error = fget(fd, &fp);
	if (error)
		goto out_error;

	error = sys_lseek(fp, offset, whence, &org);
	fdrop(fp);

	if (error)
		goto out_error;
	return org;

out_error:
	return -error;
}

static inline int has_error(int error, int bytes)
{
	/* TODO: OSv checks also for ERESTART */
	return error && (
		(bytes == 0) ||
		(error != EWOULDBLOCK && error != EINTR));
}

/**
 * Return:
 * = 0, Success and the nr of bytes read is returned in bytes parameter.
 * < 0, error code.
 */
static ssize_t do_preadv(struct vfscore_file *fp, const struct iovec *iov,
			 int iovcnt, off_t offset, ssize_t *bytes)
{
	size_t cnt;
	int error;

	UK_ASSERT(fp && iov);

	/* Otherwise, try to read the file. */
	error = sys_read(fp, iov, iovcnt, offset, &cnt);

	if (has_error(error, cnt))
		goto out_error;

	*bytes = cnt;
	return 0;

out_error:
	return -error;
}

int syscall_handler_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
	struct vfscore_file *fp;
	ssize_t bytes = 0;
	int error;

	error = fget(fd, &fp);
	if (error) {
		error = -error;
		goto out_error;
	}

	/* Check if the file is indeed seekable. */
	if (fp->f_vfs_flags & UK_VFSCORE_NOPOS) {
		error = -ESPIPE;
		goto out_error_fdrop;
	}
	/* Check if the file has not already been read and that
	 * is not a character device.
	 */
	else if (fp->f_offset < 0 &&
		(fp->f_dentry == NULL ||
		 fp->f_dentry->d_vnode->v_type != VCHR)) {
		error = -EINVAL;
		goto out_error_fdrop;
	}

	/* Otherwise, try to read the file. */
	error = do_preadv(fp, iov, iovcnt, offset, &bytes);

out_error_fdrop:
	fdrop(fp);

	if (error < 0)
		goto out_error;

	return bytes;

out_error:
	return error;
}

int syscall_handler_pread64(int fd, void *buf, size_t count, off_t offset)
{
	struct iovec iov = {
			.iov_base	= buf,
			.iov_len	= count,
	};
	ssize_t bytes;

	bytes = syscall_handler_preadv(fd, &iov,
				    1, offset);
	return bytes;
}

int syscall_handler_readv(int fd, const struct iovec *iov, int iovcnt)
{
	struct vfscore_file *fp;
	ssize_t bytes = 0;
	int error;

	error = fget(fd, &fp);
	if (error) {
		error = -error;
		goto out_error;
	}

	/* Check if the file has not already been read and that is
	 * not a character device.
	 */
	if (fp->f_offset < 0 &&
	   (fp->f_dentry == NULL ||
	    fp->f_dentry->d_vnode->v_type != VCHR)) {
		error = -EINVAL;
		goto out_error_fdrop;
	}

	/* Otherwise, try to read the file. */
	error = do_preadv(fp, iov, iovcnt, -1, &bytes);

out_error_fdrop:
	fdrop(fp);

	if (error < 0)
		goto out_error;

	return bytes;

out_error:
	return error;
}

int syscall_handler_read(int fd, void *buf, size_t count)
{
	ssize_t bytes;

	struct iovec iov = {
			.iov_base	= buf,
			.iov_len	= count,
	};

	bytes = syscall_handler_readv(fd, &iov, 1);
	return bytes;
}

static int do_pwritev(struct vfscore_file *fp, const struct iovec *iov,
		      int iovcnt, off_t offset, ssize_t *bytes)
{
	int error;
	size_t cnt;

	/* Otherwise, try to read the file. */
	error = sys_write(fp, iov, iovcnt, offset, &cnt);

	if (has_error(error, cnt))
		goto out_error;

	*bytes = cnt;
	return 0;

out_error:
	return -error;
}

int syscall_handler_pwritev(int fd, const struct iovec *iov,
	int iovcnt, off_t offset)
{
	struct vfscore_file *fp;
	ssize_t bytes = 0;
	int error;

	error = fget(fd, &fp);
	if (error) {
		error = -error;
		goto out_error;
	}

	/* Check if the file is indeed seekable. */
	if (fp->f_vfs_flags & UK_VFSCORE_NOPOS) {
		error = -ESPIPE;
		goto out_error_fdrop;
	}
	/* Check if the file has not already been written to and that it is
	 * not a character device.
	 */
	else if (fp->f_offset < 0 &&
		(fp->f_dentry == NULL ||
		 fp->f_dentry->d_vnode->v_type != VCHR)) {
		error = -EINVAL;
		goto out_error_fdrop;
	}

	/* Otherwise, try to read the file. */
	error = do_pwritev(fp, iov, iovcnt, offset, &bytes);

out_error_fdrop:
	fdrop(fp);

	if (error < 0)
		goto out_error;

	return bytes;

out_error:
	return error;
}

int syscall_handler_pwrite64(int fd, const void *buf, size_t count, off_t offset)
{
	struct iovec iov = {
			.iov_base	= (void *)buf,
			.iov_len	= count,
	};
	ssize_t bytes;

	bytes = syscall_handler_pwritev(fd, &iov,
				1, offset);
	return bytes;
}

int syscall_handler_writev(int fd, const struct iovec *vec, int vlen)
{
	struct vfscore_file *fp;
	ssize_t bytes = 0;
	int error;

	show(fd);

	error = fget(fd, &fp);
	if (error) {
		error = -error;
		goto out_error;
	}

	/* Check if the file has not already been written to and
	 * that it is not a character device.
	 */
	if (fp->f_offset < 0 &&
	   (fp->f_dentry == NULL ||
	    fp->f_dentry->d_vnode->v_type != VCHR)) {
		error = -EINVAL;
		goto out_error_fdrop;
	}

	/* Otherwise, try to read the file. */
	error = do_pwritev(fp, vec, vlen, -1, &bytes);

out_error_fdrop:
	fdrop(fp);

	if (error < 0)
		goto out_error;

	return bytes;

out_error:
	return error;
}

int syscall_handler_write(int fd, const void *buf, size_t count)
{
	show(fd);
	show(buf);
	show(count);

	ssize_t bytes;

	struct iovec iov = {
			.iov_base	= (void *)buf,
			.iov_len	= count
	};
	bytes = syscall_handler_writev(fd, &iov, 1);
	return bytes;
}

__unused int syscall_handler_ioctl(int fd, unsigned long int request, void *arg)
{
	struct vfscore_file *fp;
	int error;

	/* glibc ABI provides a variadic prototype for ioctl so we need to agree
	 * with it, since we now include sys/ioctl.h
	 * read the first argument and pass it to sys_ioctl() */

	error = fget(fd, &fp);
	if (error)
		goto out_errno;

	error = sys_ioctl(fp, request, arg);
	fdrop(fp);

	if (error)
		goto out_errno;
	return 0;

out_errno:
	return -error;
}

int fstat(int fd, struct stat *st)
{
	struct vfscore_file *fp;
	int error;

	error = fget(fd, &fp);
	if (error)
		goto out_errno;

	error = sys_fstat(fp, st);
	fdrop(fp);

	if (error)
		goto out_errno;
	return 0;

out_errno:
	errno = error;
	return -1;
}

int syscall_handler_fstat(int fd, struct stat *st)
{
	return fstat(fd, st);
}

int stat(const char *pathname, struct stat *st)
{
	struct task *t = main_task;
	char path[PATH_MAX];
	int error;

	error = task_conv(t, pathname, 0, path);
	if (error)
		goto out_errno;

	error = sys_stat(path, st);
	if (error)
		goto out_errno;
	return 0;

out_errno:
	return -error;
}

int lstat(const char *pathname, struct stat *st)
{
	struct task *t = main_task;
	char path[PATH_MAX];
	int error;

	error = task_conv(t, pathname, 0, path);
	if (error) {
		goto out_error;
	}

	error = sys_lstat(path, st);
	if (error) {
		goto out_error;
	}

	return 0;

out_error:
	return -error;
}

int syscall_handler_fstatat(int dirfd, const char *pathname, struct stat *st, int flags)
{
	if (pathname[0] == '/' || dirfd == AT_FDCWD) {
		return stat(pathname, st);
	}
	// If AT_EMPTY_PATH and pathname is an empty string, fstatat() operates on
	// dirfd itself, and in that case it doesn't have to be a directory.
	if ((flags & AT_EMPTY_PATH) && !pathname[0]) {
		return fstat(dirfd, st);
	}

	struct vfscore_file *fp;
	int error = fget(dirfd, &fp);
	if (error) {
		errno = error;
		return -1;
	}

	char p[PATH_MAX];
	/* build absolute path */
	strlcpy(p, fp->f_dentry->d_mount->m_path, PATH_MAX);
	strlcat(p, fp->f_dentry->d_path, PATH_MAX);
	strlcat(p, "/", PATH_MAX);
	strlcat(p, pathname, PATH_MAX);

	if (flags & AT_SYMLINK_NOFOLLOW)
		error = lstat(p, st);
	else
		error = stat(p, st);

	fdrop(fp);

	return error;
}


// todo
// add to init
static struct task _main_task_impl;
void vfscore_init(void)
{
	memset(&_main_task_impl, 0, sizeof(_main_task_impl));
	strcpy(_main_task_impl.t_cwd, "/");
	main_task = &_main_task_impl;

	vnode_init();
	lookup_init();
}
