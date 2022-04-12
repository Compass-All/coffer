#pragma once

#include "file.h"

int fdalloc(struct vfscore_file *fp, int *newfd);
int fget(int fd, struct vfscore_file **out_fp);
struct vfscore_file *vfscore_get_file(int fd);
int vfscore_put_fd(int fd);

void fdtable_init(void);