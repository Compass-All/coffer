#include "syscalls.h"
#include <sys/stat.h>
#include <emodules/emod_vfs/iovec.h>

// readv, openat, close, lseek, read, writev, ioctl, fstat

int	syscall_handler_open(const char *pathname, int flags, mode_t mode)
{
	return 0;
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
