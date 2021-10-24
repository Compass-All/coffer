#pragma once

#include <stddef.h>
#include <stdint.h>

void memcpy(void* dst, const void* src, size_t len);
void memset(void* dst, uint8_t byte, size_t len);