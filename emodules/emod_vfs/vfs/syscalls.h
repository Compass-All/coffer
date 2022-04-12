#pragma once

#include "file.h"

int sys_open(char *path, int flags, mode_t mode, struct vfscore_file **fpp);