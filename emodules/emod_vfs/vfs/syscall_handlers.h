#pragma once

#include <emodules/emod_vfs/iovec.h>
#include <stddef.h>
#include <sys/types.h>
#include "stat.h"
#include "dirent.h"

int	syscall_handler_open(const char *pathname, int flags, mode_t mode);
int syscall_handler_openat(int dirfd, const char *pathname, int flags, int mode);
int syscall_handler_close(int fd);
int syscall_handler_lseek(int fd, off_t offset, int whence);
int syscall_handler_preadv(int fd, const struct iovec *iov,
	int iovcnt, off_t offset);
int syscall_handler_pread64(int fd, void *buf, size_t count, off_t offset);
int syscall_handler_readv(int fd, const struct iovec *iov, int iovcnt);
int syscall_handler_read(int fd, void *buf, size_t count);
int syscall_handler_pwritev(int fd, const struct iovec *iov,
	int iovcnt, off_t offset);
int syscall_handler_pwrite64(int fd, const void *buf, size_t count, off_t offset);
int syscall_handler_writev(int fd, const struct iovec *iov, int vlen);
int syscall_handler_write(int fd, const void *buf, size_t count);
// int syscall_handler_ioctl(int fd, unsigned long int request, void *arg);
int syscall_handler_fstat(int fd, struct stat *st);
int syscall_handler_fstatat(int dirfd, const char *path, struct stat *st, int flags);
int syscall_handler_mkdirat(int dirfd, const char *pathname, mode_t mode);
int syscall_handler_fcntl(int fd, unsigned int cmd, int arg);
int syscall_handler_getdents(int fd, struct dirent* dirp, size_t count);
char *syscall_handler_getcwd(char* path, size_t size);
int syscall_handler_fsync(int fd);
int syscall_handler_unlinkat(int dirfd, const char *pathname);
int syscall_handler_ftruncate(int fd, off_t length);

void vfscore_init(void);