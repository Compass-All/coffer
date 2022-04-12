#include "syscall_handlers.h"
#include "syscalls.h"
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <emodules/emod_vfs/iovec.h>
#include "task.h"
#include "file.h"
#include "fd.h"
#include "vnode.h"

// readv, openat, close, lseek, read, writev, ioctl, fstat

struct task *main_task; /* we only have a single process */

int	syscall_handler_open(const char *pathname, int flags, mode_t mode)
{
	struct task *t = main_task;
	char path[PATH_MAX];
	__unused struct vfscore_file *fp;
	__unused int fd, error;
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

	return fd;

out_fput:
	fdrop(fp);

out_error:
	return -error;
}

int syscall_handler_openat(int dirfd, const char *pathname, int flags, int mode)
{
	return 0;
}

int syscall_handler_close(int fd)
{
	return 0;
}

int syscall_handler_lseek(int fd, off_t offset, int whence)
{
	return 0;
}

int syscall_handler_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
	return 0;
}

int syscall_handler_pread64(int fd, void *buf, size_t count, off_t offset)
{
	return 0;
}

int syscall_handler_readv(int fd, const struct iovec *iov, int iovcnt)
{
	return 0;
}

int syscall_handler_read(int fd, void *buf, size_t count)
{
	return 0;
}

int syscall_handler_pwritev(int fd, const struct iovec *iov,
	int iovcnt, off_t offset)
{
	return 0;
}

int syscall_handler_pwrite64(int fd, const void *buf, size_t count, off_t offset)
{
	return 0;
}

int syscall_handler_writev(int fd, const struct iovec *iov, int vlen)
{
	return 0;
}

int syscall_handler_write(int fd, const void *buf, size_t count)
{
	return 0;
}

int syscall_handler_ioctl(int fd, unsigned long int request, void *arg)
{
	return 0;
}

int syscall_handler_fstat(int fd, struct stat *st)
{
	return 0;
}

int syscall_handler_fstatat(int dirfd, const char *path, struct stat *st, int flags)
{
	return 0;
}
