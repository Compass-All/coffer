#pragma once

// original from unikraft

#include "../vfs/prex.h"
#include "types.h"

/*
 * File/directory node for RAMFS
 */
struct ramfs_node {
	struct ramfs_node *rn_next;   /* next node in the same directory */
	struct ramfs_node *rn_child;  /* first child node */
	int rn_type;    /* file or directory */
	char *rn_name;    /* name (null-terminated) */
	size_t rn_namelen;    /* length of name not including terminator */
	size_t rn_size;    /* file size */
	char *rn_buf;    /* buffer to the file data */
	size_t rn_bufsize;    /* allocated buffer size */
	struct timespec rn_ctime;
	struct timespec rn_atime;
	struct timespec rn_mtime;
	int rn_mode;
	bool rn_owns_buf;
};

struct ramfs_node *ramfs_allocate_node(const char *name, int type);

void ramfs_free_node(struct ramfs_node *node);

#define RAMFS_NODE(vnode) ((struct ramfs_node *) vnode->v_data)

// customized
void ramfs_vnops_init();
void init_ramfs();