#include "dentry.h"
#include <util/list.h>
#include "vnode.h"
#include "../dependency.h"
#include "string.h"

#define DENTRY_BUCKETS 32

static struct uk_hlist_head fake;
static struct uk_hlist_head dentry_hash_table[DENTRY_BUCKETS];
// static UK_HLIST_HEAD(fake);
// static struct uk_mutex dentry_hash_lock = UK_MUTEX_INITIALIZER(dentry_hash_lock);

static unsigned int
dentry_hash(struct mount *mp, const char *path)
{
	unsigned int val = 0;

	if (path) {
		while (*path) {
			val = ((val << 5) + val) + *path++;
		}
	}
	return (val ^ (unsigned long) mp) & (DENTRY_BUCKETS - 1);
}

struct dentry *dentry_alloc(struct dentry *parent_dp,
	struct vnode *vp, const char *path)
{
	struct mount *mp = vp->v_mount;
	struct dentry *dp = (struct dentry*)calloc(sizeof(*dp), 1);

	if (!dp) {
		return NULL;
	}

	dp->d_path = strdup(path);
	if (!dp->d_path) {
		free(dp);
		return NULL;
	}

	vref(vp);

	dp->d_refcnt = 1;
	dp->d_vnode = vp;
	dp->d_mount = mp;
	UK_INIT_LIST_HEAD(&dp->d_child_list);

	if (parent_dp) {
		dref(parent_dp);

		// Insert dp into its parent's children list.
		uk_list_add(&dp->d_child_link, &parent_dp->d_child_list);
	}
	dp->d_parent = parent_dp;

	vn_add_name(vp, dp);

	uk_hlist_add_head(&dp->d_link,
			  &dentry_hash_table[dentry_hash(mp, path)]);
	return dp;
}

void dref(struct dentry *dp)
{
	dp->d_refcnt++;
}

void drele(struct dentry *dp)
{
	if (--dp->d_refcnt) {
		return;
	}
	uk_hlist_del(&dp->d_link);
	vn_del_name(dp->d_vnode, dp);

	if (dp->d_parent) {
		// Remove dp from its parent's children list.
		uk_list_del(&dp->d_child_link);

		drele(dp->d_parent);
	}

	vrele(dp->d_vnode);

	free(dp->d_path);
	free(dp);
}

struct dentry *dentry_lookup(struct mount *mp, char *path)
{
	struct dentry *dp;

	uk_hlist_for_each_entry(dp, &dentry_hash_table[dentry_hash(mp, path)], d_link) {
		if (dp->d_mount == mp && !strncmp(dp->d_path, path, PATH_MAX)) {
			dp->d_refcnt++;
			return dp;
		}
	}
	return NULL;                /* not found */
}

void dentry_remove(struct dentry *dp)
{
	uk_hlist_del(&dp->d_link);
	/* put it on a fake list for drele() to work*/
	uk_hlist_add_head(&dp->d_link, &fake);
}

void dentry_init(void)
{
	int i;

	for (i = 0; i < DENTRY_BUCKETS; i++) {
		UK_INIT_HLIST_HEAD(&dentry_hash_table[i]);
	}

	UK_INIT_HLIST_HEAD(&fake);
}
