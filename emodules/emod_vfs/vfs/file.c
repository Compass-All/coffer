#include "file.h"
#include "../dependency.h"
#include "dentry.h"
#include "fops.h"

int fdrop(struct vfscore_file *fp)
{
	int prev;

	// UK_ASSERT(fp);
	// UK_ASSERT(fp->f_count > 0);

	// prev = ukarch_dec(&fp->f_count);
	prev = fp->f_count--;

	if (prev == 0)
		debug("Unbalanced fhold/fdrop");

	if (prev == 1) {
		/*
		 * we free the file even in case of an error
		 * so release the dentry too
		 */
		if (vfs_close(fp) != 0)
			drele(fp->f_dentry);

		free(fp);

		return 1;
	}

	return 0;
}

void fhold(struct vfscore_file *fp)
{
	// ukarch_inc(&fp->f_count);
	// show(fp);
	// debug("before\n");
	// show(fp->f_count);

	fp->f_count++;
	
	// debug("after\n");
	// show(fp->f_count);
}
