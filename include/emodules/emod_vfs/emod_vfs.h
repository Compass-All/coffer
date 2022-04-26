#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>

#include <sys/stat.h>
#include <emodules/emod_vfs/iovec.h>
#include <stddef.h>
#include <sys/types.h>

typedef struct {
	int	(*syscall_handler_open)(const char *pathname, int flags, mode_t mode);
	int (*syscall_handler_openat)(int dirfd, const char *pathname,
		int flags, int mode);
	int (*syscall_handler_close)(int fd);
	int (*syscall_handler_lseek)(int fd, off_t offset, int whence);

	int (*syscall_handler_preadv)(int fd, const struct iovec *iov,
		int iovcnt, off_t offset);
	int (*syscall_handler_pread64)(int fd, void *buf, size_t count, off_t offset);
	int (*syscall_handler_readv)(int fd, const struct iovec *iov, int iovcnt);
	int (*syscall_handler_read)(int fd, void *buf, size_t count);

	int (*syscall_handler_pwritev)(int fd, const struct iovec *iov,
		int iovcnt, off_t offset);
	int (*syscall_handler_pwrite64)(int fd, const void *buf, size_t count,
		off_t offset);
	int (*syscall_handler_writev)(int fd, const struct iovec *iov, int vlen);
	int (*syscall_handler_write)(int fd, const void *buf, size_t count);

	// int (*syscall_handler_ioctl)(int fd, unsigned long int request, void *arg);

	int (*syscall_handler_fstat)(int fd, struct stat *st);
	int (*syscall_handler_fstatat)(int dirfd, const char *path, struct stat *st,
		int flags);

	int (*syscall_handler_mkdirat)(int dirfd, const char *pathname, mode_t mode);

	void *(*syscall_handler_mmap)(void *addr, size_t len, int prot, int flags,
		int fildes, off_t off);

} emod_vfs_api_t;

typedef struct {
	emod_desc_t 	emod_vfs_desc;
	emod_vfs_api_t 	emod_vfs_api;
} emod_vfs_t;