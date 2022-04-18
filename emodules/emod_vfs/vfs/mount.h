#pragma once

#include "dentry.h"
#include "statfs.h"
#include "limits.h"
#include "vnode.h"

/*
 * Filesystem type switch table.
 */
struct vfscore_fs_type {
	const char      *vs_name;	/* name of file system */
	int		(*vs_init)(void); /* initialize routine */
	struct vfsops	*vs_op;		/* pointer to vfs operation */
};

// #define UK_FS_REGISTER(fssw) static struct vfscore_fs_type	
// 	__attribute((__section__(".uk_fs_list")))		
// 	*__ptr_##fssw __used = &fssw;				

/*
 * Mount flags.
 */
#ifndef MNT_RDONLY
#define	MNT_RDONLY	0x00000001	/* read only filesystem */
#endif
#ifndef	MNT_SYNCHRONOUS
#define	MNT_SYNCHRONOUS	0x00000002	/* file system written synchronously */
#endif
#ifndef	MNT_NOEXEC
#define	MNT_NOEXEC	0x00000004	/* can't exec from filesystem */
#endif
#ifndef	MNT_NOSUID
#define	MNT_NOSUID	0x00000008	/* don't honor setuid bits on fs */
#endif
#ifndef	MNT_NODEV
#define	MNT_NODEV	0x00000010	/* don't interpret special files */
#endif
#ifndef	MNT_UNION
#define	MNT_UNION	0x00000020	/* union with underlying filesystem */
#endif
#ifndef	MNT_ASYNC
#define	MNT_ASYNC	0x00000040	/* file system written asynchronously */
#endif

#define FSMAXNAMES	16		/* max length of 'file system' name */

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

#define UK_FS_REGISTER(fssw) static struct vfscore_fs_type	\
	__attribute((__section__(".uk_fs_list")))				\
	*__ptr_##fssw __used = &fssw;							\

/*
 * VFS interface
 */
#define VFS_MOUNT(MP, DEV, FL, DAT) ((MP)->m_op->vfs_mount)(MP, DEV, FL, DAT)
#define VFS_UNMOUNT(MP, FL)         ((MP)->m_op->vfs_unmount)(MP, FL)
#define VFS_SYNC(MP)                ((MP)->m_op->vfs_sync)(MP)
#define VFS_VGET(MP, VP)            ((MP)->m_op->vfs_vget)(MP, VP)
#define VFS_STATFS(MP, SFP)         ((MP)->m_op->vfs_statfs)(MP, SFP)

void vfs_unbusy(struct mount *mp);
void vfs_busy(struct mount *mp);
int vfs_findroot(const char *path, struct mount **mp, char **root);
int mount(const char *dev, const char *dir, const char *fsname,
	unsigned long flags, const void *data);
void register_fs(struct vfscore_fs_type *fs);
void mount_init();