#include <types.h>
#include <stdint.h>

#ifndef __PROXY_SYSCALLS_H__
#define __PROXY_SYSCALLS_H__

#define SYS_epoll_create1   20
#define SYS_epoll_ctl       21
#define SYS_epoll_pwait     22
#define SYS_fcntl           25
#define SYS_close 			57	// 0x39
#define SYS_pipe2           59

#define SYS_fstatat 		79

#define SYS_socket 			198
#define SYS_socketpair 		199
#define SYS_bind 			200
#define SYS_listen 			201
#define SYS_accept 			202
#define SYS_connect 		203
#define SYS_getsockname 	204
#define SYS_getpeername 	205
#define SYS_sendto 			206
#define SYS_recvfrom 		207
#define SYS_setsockopt 		208
#define SYS_getsockopt 		209
#define SYS_shutdown 		210
#define SYS_sendmsg 		211
#define SYS_recvmsg 		212
#define SYS_readahead 		213
#define SYS_accept4 		242


#define SHARED_BUFFER_START 0x2000000000UL
// #define PARTITION_SIZE 1 << 21UL

typedef size_t proxy_data_offset;

struct proxy_syscall {
	size_t syscall_num;
	unsigned char data[];
};

enum proxy_syscall_status{
    CALL_STATUS_OK             = 0,
    CALL_STATUS_BAD_CALL_ID    = 1,
    CALL_STATUS_BAD_OFFSET     = 2,
    CALL_STATUS_BAD_PTR        = 3,
    CALL_STATUS_ERROR          = 4,
    CALL_STATUS_SYSCALL_FAILED = 5,
    CALL_STATUS_ONGOING        = 6,
};

struct proxy_syscall_return {
    /* Status variable indicating error/success conditions. Not for data
        values. */
    unsigned long call_status;

    /* OFFSET into the shared memory region.  Should be checked for
    * validity, then turned into a pointer to a relevant return data
    * structure for the call. (User/call defined) */
    proxy_data_offset call_ret_offset;
    size_t call_ret_size;
};

struct proxy_info {
    /* Similar to syscall number.  User-defined call id, handled at the
    * edges only */
    // unsigned long call_id;

    /* OFFSET into the shared memory region.  Should be checked for
    * validity, then turned into a pointer to a relevant argument
    * structure for the call. (User/call defined)*/
    proxy_data_offset call_arg_offset;
    size_t call_arg_size;

    /* Pre-set location to structure return data */
    struct proxy_syscall_return return_data;
};

uintptr_t proxy_syscall_data_ptr();

int proxy_syscall_get_ptr_from_offset(
    proxy_data_offset offset,
    size_t data_len,
    uintptr_t* ptr
);

int proxy_syscall_check_ptr_valid(
    uintptr_t ptr,
    size_t data_len
);

int proxy_syscall_get_offset_from_ptr(
    uintptr_t ptr,
    size_t data_len,
    proxy_data_offset* offset
);

int proxy_syscall_args_ptr(
    struct proxy_info* proxy_info,
    uintptr_t* ptr,
    size_t* size
);

int proxy_syscall_ret_ptr(
    struct proxy_info* proxy_info,
    uintptr_t* ptr,
    size_t* size
);

int proxy_syscall_setup_call(
    struct proxy_info* proxy_info,
    void* ptr,
    size_t size
);

int proxy_syscall_setup_ret(
    struct proxy_info* proxy_info,
    void* ptr,
    size_t size
);

uintptr_t proxy_syscall_to_host(
    struct proxy_syscall* syscall_data_ptr,
    size_t data_len
);

// extern uint64_t shared_buffer_pa;

#endif