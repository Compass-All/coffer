#pragma once

#include <types.h>
#include <emodules/emodule_desc.h>
#include <emodules/emodule_id.h>
#include <sys/types.h>
#include "socket.h"
#include "epoll.h"

typedef struct sargs_SYS_socket{
  int domain;
  int type;
  int protocol;
} sargs_SYS_socket;

// typedef struct sargs_SYS_socketpair{
// }

typedef struct sargs_SYS_bind{
  int sockfd;
  struct sockaddr_storage addr;
  socklen_t addrlen;
} sargs_SYS_bind;

typedef struct sargs_SYS_listen{
  int sockfd;
  int backlog;
} sargs_SYS_listen;

typedef struct sargs_SYS_accept{
  int sockfd;
  struct sockaddr_storage addr;
  socklen_t addrlen;
} sargs_SYS_accept;

typedef struct sargs_SYS_connect{
  int sockfd;
  struct sockaddr_storage addr;
  socklen_t addrlen;
} sargs_SYS_connect;

typedef struct sargs_SYS_getsockname{
  int sockfd;
  struct sockaddr addr;
  socklen_t addrlen;
} sargs_SYS_getsockname;

typedef struct sargs_SYS_getpeername{
  int sockfd;
  struct sockaddr_storage addr;
  socklen_t addrlen;
} sargs_SYS_getpeername;

typedef struct sargs_SYS_sendto {
  int sockfd; 
  size_t len; 
  int flags; 
  int dest_addr_is_null; 
  struct sockaddr dest_addr; 
  socklen_t addrlen; 
  char buf[]; 
} sargs_SYS_sendto;

typedef struct sargs_SYS_recvfrom {
  int sockfd; 
  size_t len; 
  int flags; 
  int src_addr_is_null; 
  struct sockaddr src_addr; 
  socklen_t addrlen; 
  char buf[]; 
} sargs_SYS_recvfrom;

typedef struct sargs_SYS_setsockopt{
  int socket;
  int level;
  int option_name;
  socklen_t option_len;
  unsigned char option_value[];
} sargs_SYS_setsockopt;

// // typedef struct sargs_SYS_getsockopt{
// // }

typedef struct sargs_SYS_epoll_create1 {
  int size;
} sargs_SYS_epoll_create1;

typedef struct sargs_SYS_epoll_ctl{
  int epfd;
  int op;
  int fd;
  uintptr_t event;
} sargs_SYS_epoll_ctl;

typedef struct sargs_SYS_epoll_pwait{
  int epfd;
  struct epoll_event events;
  int maxevents;
  int timeout;
} sargs_SYS_epoll_pwait;

typedef struct sargs_SYS_fcntl {
  int fd;
  int cmd;
  int has_struct;
  unsigned long arg[];
} sargs_SYS_fcntl;

struct _sargs_fd_only {
  int fd;
};

typedef struct _sargs_fd_only sargs_SYS_close;
