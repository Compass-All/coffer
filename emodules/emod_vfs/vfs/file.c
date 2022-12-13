#include "file.h"
#include "../dependency.h"
#include "dentry.h"
#include "fops.h"

int fdrop(struct vfscore_file *fp)
{
	int prev;

	info("droping fp (%p), count %d to %d\n",
		fp, fp->f_count, fp->f_count - 1);
	prev = fp->f_count--;

	if (prev == 0) {
		panic("Unbalanced fhold/fdrop\n");
	}

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
	info("holding fp (%p), count %d to %d\n",
		fp, fp->f_count, fp->f_count + 1);
	fp->f_count++;
}
