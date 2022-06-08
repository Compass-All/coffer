#pragma once

#include "dentry.h"

int namei(const char *path, struct dentry **dpp);
int lookup(char *path, struct dentry **dpp, char **name);
int namei_last_nofollow(char *path, struct dentry *ddp, struct dentry **dpp);
int namei_follow_link(struct dentry *dp, char *node, char *name,
	char *fp, size_t mountpoint_len);
void lookup_init(void);