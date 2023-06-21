/**
 * Taken and modified from Unikraft.
 *
 * Taken and modified from Linux Kernel.
 * include/uapi/linux/virtio_types.h
 *
 * Commit-id: 9efeccacd3a4
 */
#ifndef __PLAT_DRV_VIRTIO_TYPES_H
#define __PLAT_DRV_VIRTIO_TYPES_H

#include <types.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

/**
 * __virtio{16,32,64} have the following meaning:
 * - __u{16,32,64} for virtio devices in legacy mode, accessed in native endian
 * - __le{16,32,64} for standard-compliant virtio devices
 */
typedef __u16 __virtio_le16;
typedef __u32 __virtio_le32;
typedef __u64 __virtio_le64;

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* __PLAT_DRV_VIRTIO_TYPES_H */
