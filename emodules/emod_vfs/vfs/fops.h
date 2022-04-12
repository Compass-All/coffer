#pragma once

#include "uio.h"
#include "file.h"
#include "stat.h"

int vfs_read(struct vfscore_file *fp, struct uio *uio, int flags);
int vfs_write(struct vfscore_file *fp, struct uio *uio, int flags);
int vfs_ioctl(struct vfscore_file *fp, unsigned long com, void *data);
int vfs_stat(struct vfscore_file *fp, struct stat *st);