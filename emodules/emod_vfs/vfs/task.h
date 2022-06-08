#pragma once

#include "limits.h"

struct task {
	char 	t_cwd[PATH_MAX];			/* current working directory */
	struct 	vfscore_file *t_cwdfp;		/* directory for cwd */
};

int task_conv(struct task *t, const char *cpath, int acc __unused, char *full);
int path_conv(char *wd, const char *cpath, char *full);