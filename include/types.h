#pragma once

typedef char s8;
typedef unsigned char u8;

typedef short s16;
typedef unsigned short u16;
typedef unsigned short ushort;
typedef unsigned short uint16_t;

typedef int s32;
typedef unsigned int u32;
typedef unsigned int uint;

typedef long s64;
typedef unsigned long u64;
typedef unsigned long ulong;

#ifndef __GNUG__ // GNU C++
typedef int bool;
#endif

typedef u64 usize;
typedef unsigned long size_t;

typedef u64 paddr_t;
typedef u64 vaddr_t;