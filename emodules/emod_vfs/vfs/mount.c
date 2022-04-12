#include "mount.h"
#include "list.h"

UK_LIST_HEAD(mount_list);

void vfs_unbusy(struct mount *mp)
{
	/* The m_count is not really checked anywhere
	 * currently. Atomic is enough. But it could be that obtaining
	 * mount_lock will be needed in the future */
	// ukarch_dec(&mp->m_count);
	mp->m_count--;
}

static size_t
count_match(const char *path, char *mount_root)
{
	size_t len = 0;

	while (*path && *mount_root) {
		if (*path != *mount_root)
			break;

		path++;
		mount_root++;
		len++;
	}
	if (*mount_root != '\0')
		return 0;

	if (len == 1 && *(path - 1) == '/')
		return 1;

	if (*path == '\0' || *path == '/')
		return len;
	return 0;
}

int vfs_findroot(const char *path, struct mount **mp, char **root)
{
	struct mount *m = NULL, *tmp;
	size_t len, max_len = 0;

	if (!path)
		return -1;

	/* Find mount point from nearest path */
	uk_list_for_each_entry(tmp, &mount_list, mnt_list) {
		len = count_match(path, tmp->m_path);
		if (len > max_len) {
			max_len = len;
			m = tmp;
		}
	}
	if (m == NULL)
		return -1;
	*root = (char *)(path + max_len);
	if (**root == '/')
		(*root)++;
	*mp = m;
	return 0;
}
