#pragma once

#define FENCE() __asm__ __volatile__ ("fence " "rw" "," "rw" : : : "memory")