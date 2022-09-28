#include "stdio.h"
#include "fd.h"
#include "../dependency.h"
#include "file.h"
#include "fs.h"
#include "dentry.h"
#include "vnode.h"
#include "mount.h"
#include "errno.h"

#define VEOF            0       /* ICANON */
#define VEOL            1       /* ICANON */

void _putc(char c)
{
	// return;
	register uintptr_t a0 asm("a0") = (uintptr_t)(c);
	register uintptr_t a7 asm("a7") = (uintptr_t)(0x1);
	asm volatile("ecall": "+r"(a0): "r"(a7): "memory");
}

int _getc()
{
	uintptr_t c;
	asm volatile(
		"li		a7, 0x2	\n\t"
		"ecall			\n\t"

		"mv		%0, a0	\n\t"
		: "=r"(c)
		:: "a0", "a7"
	);
	return (int)c;
}

static int coutk(const char *buf, unsigned int len)
{
	for (unsigned int i = 0; i < len; i++)
		_putc(buf[i]);
	return len;
}

static int cink(char *buf, unsigned int maxlen)
{
	int ret;
	unsigned int num = 0;

	while (num < maxlen && (ret = _getc()) >= 0) {
		*(buf++) = (char) ret;
		num++;
	}

	return (int) num;
}


static int __write_fn(void *dst __unused, void *src, size_t *cnt)
{
	int ret = coutk(src, *cnt);

	if (ret < 0)
		/* TODO: remove -1 when vfscore switches to negative
		 * error numbers
		 */
		return ret * -1;

	*cnt = (size_t) ret;
	return 0;
}

/* One function for stderr and stdout */
static int stdio_write(
	struct vnode *vp __unused,
	struct uio *uio,
	int ioflag __unused
)
{
	if (uio->uio_offset)
		return ESPIPE;

	return vfscore_uioforeach(__write_fn, NULL, uio->uio_resid, uio);
}

static int __read_fn(void *dst, void *src __unused, size_t *cnt)
{
	int bytes_read;
	size_t bytes_total = 0, count;
	char *buf = dst;

	count = *cnt;

	do {
		while (
			(bytes_read = 
			cink(buf, count - bytes_total)) <= 0
		);

		buf = buf + bytes_read;
		*(buf - 1) = *(buf - 1) == '\r' ?
					'\n' : *(buf - 1);

		/* Echo the input */
		if (*(buf - 1) == '\177') {
			/* DELETE control character */
			if (buf - 1 != dst) {
				/* If this is not the first byte */
				coutk("\b \b", 3);
				buf -= 1;
				bytes_total -= 1;
			}
			buf -= 1;
		} else {
			coutk(buf - bytes_read, bytes_read);
			bytes_total += bytes_read;
		}

	} while (bytes_total < count && *(buf - 1) != '\n'
			&& *(buf - 1) != VEOF);

	*cnt = bytes_total;

	/* The INT_MIN here is a special return code. It makes the
	 * vfscore_uioforeach to quit from the loop. But this is not
	 * an error (for example a user hit Ctrl-C). That is why this
	 * special return value is fixed up to 0 in the stdio_read.
	 */
	if (*(buf - 1) == '\n' || *(buf - 1) == VEOF)
		return INT_MIN;

	return 0;
}

static int stdio_read(
	struct vnode *vp __unused,
	struct vfscore_file *file __unused,
	struct uio *uio,
	int ioflag __unused
)
{
	int ret;

	if (uio->uio_offset)
		return ESPIPE;

	ret = vfscore_uioforeach(__read_fn, NULL, uio->uio_resid, uio);
	ret = (ret == INT_MIN) ? 0 : ret;

	return ret;
}

static int stdio_getattr(
	struct vnode *vnode __unused, 
	struct vattr *attr __unused
)
{
	return 0;
}

#define stdio_open	((vnop_open_t)vfscore_nullop)
#define stdio_close	((vnop_close_t)vfscore_nullop)
#define stdio_seek	((vnop_seek_t)vfscore_vop_nullop)
#define stdio_ioctl	((vnop_ioctl_t)vfscore_nullop)
#define stdio_fsync	((vnop_fsync_t)vfscore_vop_nullop)
#define stdio_readdir	((vnop_readdir_t)vfscore_vop_einval)
#define stdio_lookup	((vnop_lookup_t)vfscore_nullop)
#define stdio_create	((vnop_create_t)vfscore_vop_einval)
#define stdio_remove	((vnop_remove_t)vfscore_vop_einval)
#define stdio_rename	((vnop_rename_t)vfscore_vop_einval)
#define stdio_mkdir	((vnop_mkdir_t)vfscore_vop_einval)
#define stdio_rmdir	((vnop_rmdir_t)vfscore_vop_einval)
#define stdio_setattr	((vnop_setattr_t)vfscore_vop_eperm)
#define stdio_inactive	((vnop_inactive_t)vfscore_vop_nullop)
#define stdio_truncate	((vnop_truncate_t)vfscore_vop_nullop)
#define stdio_link	((vnop_link_t)vfscore_vop_eperm)
#define stdio_fallocate	((vnop_fallocate_t)vfscore_vop_nullop)
#define stdio_readlink	((vnop_readlink_t)vfscore_vop_nullop)
#define stdio_symlink	((vnop_symlink_t)vfscore_vop_nullop)

static struct vnops stdio_vnops;

static struct vnode stdio_vnode = {
	.v_ino = 1,
	// .v_op = &stdio_vnops,
	.v_refcnt = 1,
	.v_type = VCHR,
};

static struct dentry stdio_dentry;
//  = {
// 	.d_vnode = &stdio_vnode,
// };

static struct vfscore_file stdio_file = {
	.fd = 1,
	.f_flags = UK_FWRITE | UK_FREAD,
	// .f_dentry = &stdio_dentry,
	.f_vfs_flags = UK_VFSCORE_NOPOS,
	/* reference count is 2 because close(0) is a valid
	 * operation. However it is not properly handled in the
	 * current implementation. */
	.f_count = 2,
};


void init_stdio(void)
{
	int fd;

	stdio_vnops.vop_open		= stdio_open;
	stdio_vnops.vop_close		= stdio_close;
	stdio_vnops.vop_read		= stdio_read;
	stdio_vnops.vop_write		= stdio_write;
	stdio_vnops.vop_seek		= stdio_seek;
	stdio_vnops.vop_ioctl		= stdio_ioctl;
	stdio_vnops.vop_fsync		= stdio_fsync;
	stdio_vnops.vop_readdir		= stdio_readdir;
	stdio_vnops.vop_lookup		= stdio_lookup;
	stdio_vnops.vop_create		= stdio_create;
	stdio_vnops.vop_remove		= stdio_remove;
	stdio_vnops.vop_rename		= stdio_rename;
	stdio_vnops.vop_mkdir		= stdio_mkdir;
	stdio_vnops.vop_rmdir		= stdio_rmdir;
	stdio_vnops.vop_getattr		= stdio_getattr;
	stdio_vnops.vop_setattr		= stdio_setattr;
	stdio_vnops.vop_inactive	= stdio_inactive;
	stdio_vnops.vop_truncate	= stdio_truncate;
	stdio_vnops.vop_link		= stdio_link;
	stdio_vnops.vop_cache		= (vnop_cache_t) NULL;
	stdio_vnops.vop_fallocate	= stdio_fallocate;
	stdio_vnops.vop_readlink	= stdio_readlink;
	stdio_vnops.vop_symlink		= stdio_symlink;

	stdio_vnode.v_op		= &stdio_vnops;
	stdio_dentry.d_vnode 	= &stdio_vnode;
	stdio_file.f_dentry 	= &stdio_dentry;

	fd = vfscore_alloc_fd();

	if (fd != 0)
		panic("stdio fd error!\n");

	vfscore_install_fd(0, &stdio_file);
	if (dup2(0, 1) != 1)
		panic("failed to dup to stdin\n");
	if (dup2(0, 2) != 2)
		panic("failed to dup to stderr\n");
}