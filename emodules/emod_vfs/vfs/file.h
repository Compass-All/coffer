#pragma once

#include <sys/types.h>

struct vfscore_file {
	int 	fd;
	int		f_flags;	/* open flags */
	int		f_count;	/* reference count */
	off_t	f_offset;	/* current position in file */
	void	*f_data;        /* file descriptor specific data */
	int		f_vfs_flags;    /* internal implementation flags */
	struct dentry   *f_dentry;
	// struct uk_mutex f_lock;
};

int fdrop(struct vfscore_file *fp);
void fhold(struct vfscore_file *fp);

#define FOF_OFFSET  0x0800    /* Use the offset in uio argument */