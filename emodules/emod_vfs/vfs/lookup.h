#pragma once

#include "dentry.h"

int namei(const char *path, struct dentry **dpp);
int lookup(char *path, struct dentry **dpp, char **name);
int namei_last_nofollow(char *path, struct dentry *ddp, struct dentry **dpp);