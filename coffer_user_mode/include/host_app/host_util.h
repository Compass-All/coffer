#pragma once

#include <enclave/host_ops.h>
#include <types.h>
#include <stdio.h>

#define debug(fmt, ...) do {	\
	printf(fmt, ##__VA_ARGS__);	\
	fflush(stdout);				\
} while (0)

#define debug(fmt, ...)

usize read_file(const char *path, u8 *content[]);
u64 create_enclave();
u64 enter_enclave(u64 eid, int argc, char *argv[], const char *path);