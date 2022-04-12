#pragma once

#include "list.h"
#include "mount.h"

struct vnode;

struct dentry {
	struct uk_hlist_node d_link;	/* link for hash list */
	int		d_refcnt;	/* reference count */
	char		*d_path;	/* pointer to path in fs */
	struct vnode	*d_vnode;
	struct mount	*d_mount;
	struct dentry   *d_parent; /* pointer to parent */
	struct uk_list_head d_names_link; /* link fo vnode::d_names */
	// struct uk_mutex	d_lock;
	struct uk_list_head d_child_list;
	struct uk_list_head d_child_link;
};

void drele(struct dentry *dp);
void dref(struct dentry *dp);
struct dentry *dentry_lookup(struct mount *mp, char *path);
struct dentry *dentry_alloc(struct dentry *parent_dp,
	struct vnode *vp, const char *path);
void dentry_init(void);