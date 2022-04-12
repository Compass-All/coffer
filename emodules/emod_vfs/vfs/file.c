#include "file.h"

int fdrop(struct vfscore_file *fp)
{
	// todo
	return 0;
}

void fhold(struct vfscore_file *fp)
{
	// ukarch_inc(&fp->f_count);
	fp->f_count++;
}
