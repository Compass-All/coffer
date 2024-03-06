#pragma once

typedef char s8;
typedef unsigned char u8;
typedef unsigned char uint8_t;

typedef short s16;
typedef short int16_t;
typedef unsigned short u16;
typedef unsigned short ushort;
typedef unsigned short uint16_t;

typedef int s32;
typedef int int32_t;
typedef unsigned int u32;
typedef unsigned int uint;
typedef unsigned int uint32_t;

typedef long s64;
typedef unsigned long u64;
typedef unsigned long ulong;
typedef unsigned long uint64_t;

typedef u64 usize;
typedef unsigned long size_t;
typedef signed long ssize_t;

#ifndef _ASM_GENERIC_TYPES_H
typedef signed   char      __s8;
typedef unsigned char      __u8;
#endif
#define __PRIs8 "d"
#define __PRIu8 "u"
#define __PRIx8 "x"
#define __SCNs8 "hhd"
#define __SCNu8 "hhu"
#define __SCNx8 "hhx"
#define __HAVE_INT8__

#ifndef _ASM_GENERIC_TYPES_H
typedef signed   short     __s16;
typedef unsigned short     __u16;
#endif
#define __PRIs16 "d"
#define __PRIu16 "u"
#define __PRIx16 "x"
#define __SCNs16 "hd"
#define __SCNu16 "hu"
#define __SCNx16 "hx"
#define __HAVE_INT16__

#ifndef _ASM_GENERIC_TYPES_H
typedef signed   int     __s32;
typedef unsigned int     __u32;
#endif
#define __PRIs32 "d"
#define __PRIu32 "u"
#define __PRIx32 "x"
#define __SCNs32 "d"
#define __SCNu32 "u"
#define __SCNx32 "x"
#define __HAVE_INT32__

#ifndef _ASM_GENERIC_TYPES_H
typedef signed   long      __s64;
typedef unsigned long      __u64;
#endif
#define __PRIs64 "ld"
#define __PRIu64 "lu"
#define __PRIx64 "lx"
#define __SCNs64 "ld"
#define __SCNu64 "lu"
#define __SCNx64 "lx"
#define __HAVE_INT64__

typedef __s64 __sptr;
typedef __u64 __uptr;
typedef __u64 uintptr_t;
#define __PRIuptr __PRIx64
#define __PRIsz   __PRIu64
#define __PRIssz  __PRIs64
#define __PRIoff  __PRIs64
#define __HAVE_PTR__

typedef __uptr __sz; /* size_t equivalent */
typedef __sptr __ssz; /* ssize_t equivalent */
typedef __sptr __off; /* off_t equivalent */

typedef __uptr __vaddr_t; /* virtual address */
#define __PRIvaddr __PRIuptr

typedef __u64 __paddr_t;
#define __PRIpaddr __PRIx64
#define __HAVE_PADDR__

typedef u64 paddr_t;
typedef u64 vaddr_t;

#ifndef __NULL
#define __NULL ((void *) 0)
#endif

typedef struct {
	__u32 counter;
} __atomic;