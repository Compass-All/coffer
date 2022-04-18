#pragma once

#include "file.h"

int fdalloc(struct vfscore_file *fp, int *newfd);
int fget(int fd, struct vfscore_file **out_fp);
struct vfscore_file *vfscore_get_file(int fd);
int vfscore_put_fd(int fd);
int vfscore_alloc_fd(void);
int vfscore_install_fd(int fd, struct vfscore_file *file);
int dup3(int oldfd, int newfd, int flags);
int dup2(int oldfd, int newfd);

void fdtable_init(void);