#pragma once

#define false 0
#define true 1

void printd(const char *s, ...);

#define debug(fmt, ...) printd("\033[37m[%s] " fmt "\033[0m", __func__, ##__VA_ARGS__)