#pragma once

#include "file.h"
#include <emodules/emod_vfs/iovec.h>
#include "stat.h"

int sys_open(char *path, int flags, mode_t mode, struct vfscore_file **fpp);
int sys_close(struct vfscore_file *fp __unused);
int sys_read(struct vfscore_file *fp, const struct iovec *iov, size_t niov,
		off_t offset, size_t *count);
int sys_write(struct vfscore_file *fp, const struct iovec *iov, size_t niov,
		off_t offset, size_t *count);
int sys_lseek(struct vfscore_file *fp, off_t off, int type, off_t *origin);
int sys_ioctl(struct vfscore_file *fp, unsigned long request, void *buf);
int sys_fstat(struct vfscore_file *fp, struct stat *st);
int sys_stat(char *path, struct stat *st);
int sys_lstat(char *path, struct stat *st);
int sys_mkdir(char *path, mode_t mode);