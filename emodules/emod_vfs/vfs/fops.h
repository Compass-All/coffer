#pragma once

#include "uio.h"
#include "file.h"

int vfs_read(struct vfscore_file *fp, struct uio *uio, int flags);
int vfs_write(struct vfscore_file *fp, struct uio *uio, int flags);