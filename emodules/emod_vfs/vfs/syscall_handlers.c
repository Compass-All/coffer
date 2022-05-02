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
#include "fops.h"
#include "fs.h"
#include "errptr.h"
#include "../dependency.h"

struct __dirstream
{
	int fd;
};

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

static mode_t global_umask = S_IWGRP | S_IWOTH;

static inline mode_t apply_umask(mode_t mode)
{
	return mode & ~global_umask;
}

int syscall_handler_mkdirat(__unused int dirfd, const char *pathname, mode_t mode)
{
	struct task *t = main_task;
	char path[PATH_MAX];
	int error;

	show(mode);
	show(t);
	show(global_umask);
	show(pathname);

	mode = apply_umask(mode);

	show(mode);

	if ((error = task_conv(t, pathname, VWRITE, path)) != 0)
		goto out_errno;

	error = sys_mkdir(path, mode);

	if (error)
		goto out_errno;
	return 0;

out_errno:
	return -error;
}

/*
 * The file control system call.
 */
#define SETFL (O_APPEND | O_ASYNC | O_DIRECT | O_NOATIME | O_NONBLOCK)

int syscall_handler_fcntl(int fd, unsigned int cmd, int arg)
{
	struct vfscore_file *fp;
	int ret = 0, error;
#if defined(FIONBIO) && defined(FIOASYNC)
	int tmp;
#endif

	error = fget(fd, &fp);
	if (error)
		goto out_errno;

	// An important note about our handling of FD_CLOEXEC / O_CLOEXEC:
	// close-on-exec shouldn't have been a file flag (fp->f_flags) - it is a
	// file descriptor flag, meaning that that two dup()ed file descriptors
	// could have different values for FD_CLOEXEC. Our current implementation
	// *wrongly* makes close-on-exec an f_flag (using the bit O_CLOEXEC).
	// There is little practical difference, though, because this flag is
	// ignored in OSv anyway, as it doesn't support exec().
	switch (cmd) {
	case F_DUPFD:
		error = fdalloc(fp, &ret);
		if (error)
			goto out_errno;
		break;
	case F_GETFD:
		ret = (fp->f_flags & O_CLOEXEC) ? FD_CLOEXEC : 0;
		break;
	case F_SETFD:
		fp->f_flags = (fp->f_flags & ~O_CLOEXEC) |
				((arg & FD_CLOEXEC) ? O_CLOEXEC : 0);
		break;
	case F_GETFL:
		// As explained above, the O_CLOEXEC should have been in f_flags,
		// and shouldn't be returned. Linux always returns 0100000 ("the
		// flag formerly known as O_LARGEFILE) so let's do it too.
		ret = (vfscore_oflags(fp->f_flags) & ~O_CLOEXEC) | 0100000;
		break;
	case F_SETFL:
		fp->f_flags = vfscore_fflags((vfscore_oflags(fp->f_flags) & ~SETFL) |
				(arg & SETFL));

#if defined(FIONBIO) && defined(FIOASYNC)
		/* Sync nonblocking/async state with file flags */
		tmp = fp->f_flags & FNONBLOCK;
		vfs_ioctl(fp, FIONBIO, &tmp);
		tmp = fp->f_flags & FASYNC;
		vfs_ioctl(fp, FIOASYNC, &tmp);
#endif
		break;
	case F_DUPFD_CLOEXEC:
		error = fdalloc(fp, &ret);
		if (error)
			goto out_errno;
		fp->f_flags |= O_CLOEXEC;
		break;
	case F_SETLK:
		debug("fcntl(F_SETLK) stubbed\n");
		break;
	case F_GETLK:
		debug("fcntl(F_GETLK) stubbed\n");
		break;
	case F_SETLKW:
		debug("fcntl(F_SETLKW) stubbed\n");
		break;
	case F_SETOWN:
		debug("fcntl(F_SETOWN) stubbed\n");
		break;
	default:
		debug("unsupported fcntl cmd 0x%x\n", cmd);
		error = EINVAL;
	}

	fdrop(fp);
	if (error)
		goto out_errno;
	return ret;

out_errno:
	return -error;
}

int readdir_r(DIR *dir, struct dirent *entry, struct dirent **result)
{
	int error;
	struct vfscore_file *fp;

	error = fget(dir->fd, &fp);
	if (!error) {
		error = sys_readdir(fp, entry);
		fdrop(fp);
	}
	// Our dirent has (like Linux) a d_reclen field, but a constant size.
	entry->d_reclen = sizeof(*entry);

	if (error) {
		*result = NULL;
	} else {
		*result = entry;
	}
	return error == ENOENT ? 0 : error;
}

int syscall_handler_getdents(int fd, struct dirent* dirp, size_t count)
{
	if (dirp == NULL || count == 0)
		return 0;

	DIR dir = {
		.fd = fd
	};

	size_t i = 0;
	struct dirent entry, *result;
	int error;

	do {
		error = readdir_r(&dir, &entry, &result);
		if (error)
			return -error;

		if (result != NULL) {
			memcpy(dirp + i, result, sizeof(struct dirent));
			i++;

		} else
			break;

	} while (i < count);

	return (i * sizeof(struct dirent));
}

char *syscall_handler_getcwd(char* path, size_t size)
{
	struct task *t = main_task;
	size_t len = strlen(t->t_cwd) + 1;
	int error;

	if (size < len) {
		error = ERANGE;
		goto out_error;
	}

	if (!path) {
		if (!size)
			size = len;
		path = (char*)malloc(size);
		if (!path) {
			error = ENOMEM;
			goto out_error;
		}
	} else {
		if (!size) {
			error = EINVAL;
			goto out_error;
		}
	}

	memcpy(path, t->t_cwd, len);
	return path;

out_error:
	return ERR2PTR(-error);
}

int syscall_handler_fsync(int fd)
{
	struct vfscore_file *fp;
	int error;

	error = fget(fd, &fp);
	if (error)
		goto out_error;

	error = sys_fsync(fp);
	fdrop(fp);

	if (error)
		goto out_error;
	return 0;

out_error:
	return -error;
}

int syscall_handler_unlinkat(__unused int dirfd, const char *pathname)
{
	struct task *t = main_task;
	char path[PATH_MAX];
	int error;

	debug("pathname: %s\n", pathname);

	error = ENOENT;
	if (pathname == NULL)
		goto out_errno;
	if ((error = task_conv(t, pathname, VWRITE, path)) != 0)
		goto out_errno;

	error = sys_unlink(path);
	if (error)
		goto out_errno;
	return 0;
out_errno:
	return -error;
}

int syscall_handler_ftruncate(int fd, off_t length)
{
	struct vfscore_file *fp;
	int error;

	error = fget(fd, &fp);
	if (error)
		goto out_error;

	error = sys_ftruncate(fp, length);
	fdrop(fp);

	if (error)
		goto out_error;
	return 0;

out_error:
	return -error;
}

static struct task _main_task_impl;

void vfscore_init(void)
{
	memset(&_main_task_impl, 0, sizeof(_main_task_impl));
	strcpy(_main_task_impl.t_cwd, "/");
	main_task = &_main_task_impl;

	vnode_init();
	lookup_init();
}
