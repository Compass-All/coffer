#pragma once

#include "dentry.h"
#include "statfs.h"
#include "limits.h"

struct mount {
	struct vfsops	*m_op;		/* pointer to vfs operation */
	int		m_flags;	/* mount flag */
	int		m_count;	/* reference count */
	char            m_path[PATH_MAX]; /* mounted path */
	char            m_special[PATH_MAX]; /* resource */
	struct device	*m_dev;		/* mounted device */
	struct dentry	*m_root;	/* root vnode */
	struct dentry	*m_covered;	/* vnode covered on parent fs */
	void		*m_data;	/* private data for fs */
	struct uk_list_head mnt_list;
	fsid_t 		m_fsid; 	/* id that uniquely identifies the fs */
};

struct vfsops {
	int (*vfs_mount)	(struct mount *, const char *, int, const void *);
	int (*vfs_unmount)	(struct mount *, int flags);
	int (*vfs_sync)		(struct mount *);
	int (*vfs_vget)		(struct mount *, struct vnode *);
	int (*vfs_statfs)	(struct mount *, struct statfs *);
	struct vnops	*vfs_vnops;
};
