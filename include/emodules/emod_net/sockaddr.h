#ifndef _BITS_SOCKADDR_H
#define _BITS_SOCKADDR_H	1

/* POSIX.1g specifies this type name for the `sa_family' member.  */
typedef unsigned short int sa_family_t;

#define	__SOCKADDR_COMMON(sa_prefix) \
    sa_family_t sa_prefix##family

#define __SOCKADDR_COMMON_SIZE	(sizeof (unsigned short int))

/* Size of struct sockaddr_storage.  */
#define _SS_SIZE 128

#endif	/* bits/sockaddr.h */
