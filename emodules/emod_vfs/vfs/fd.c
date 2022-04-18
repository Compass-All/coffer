#include "fd.h"
#include "file.h"
#include "errno.h"
#include "bitops.h"
#include "bitmap.h"
#include "stdio.h"
#include "../dependency.h"
#include "fcntl.h"
#include "syscall_handlers.h"

void init_stdio(void);

struct fdtable {
	unsigned long 		bitmap[UK_BITS_TO_LONGS(FDTABLE_MAX_FILES)];
	uint32_t 			fd_start;
	struct vfscore_file *files[FDTABLE_MAX_FILES];
};
struct fdtable fdtable;

static void dump_fdtable()
{
	show(fdtable.fd_start);
	for (int i = 0; i < sizeof(fdtable.bitmap) / sizeof(unsigned long); i++) {
		show(i);
		show(fdtable.bitmap[i]);
	}

	for (int i = 0; i < FDTABLE_MAX_FILES; i++) {
		if (fdtable.files[i])
			show(fdtable.files[i]);
	}
}

int vfscore_alloc_fd(void)
{
	// unsigned long flags;
	int ret;

	// flags = ukplat_lcpu_save_irqf();
	ret = uk_find_next_zero_bit(fdtable.bitmap, FDTABLE_MAX_FILES, 0);

	if (ret == FDTABLE_MAX_FILES) {
		ret = -ENFILE;
		goto exit;
	}

	uk_bitmap_set(fdtable.bitmap, ret, 1);

exit:
	// ukplat_lcpu_restore_irqf(flags);
	return ret;
}

int vfscore_install_fd(int fd, struct vfscore_file *file)
{
	// unsigned long flags;
	struct vfscore_file *orig;

	if ((fd >= (int) FDTABLE_MAX_FILES) || (!file))
		return -EBADF;

	fhold(file);

	file->fd = fd;

	// flags = ukplat_lcpu_save_irqf();
	orig = fdtable.files[fd];
	fdtable.files[fd] = file;
	// ukplat_lcpu_restore_irqf(flags);

	fdrop(file);

	if (orig)
		fdrop(orig);

	return 0;
}

int fdalloc(struct vfscore_file *fp, int *newfd)
{
	int fd, ret = 0;

	fhold(fp);

	fd = vfscore_alloc_fd();
	if (fd < 0) {
		ret = fd;
		goto exit;
	}

	ret = vfscore_install_fd(fd, fp);
	if (ret)
		fdrop(fp);
	else
		*newfd = fd;

exit:
	return ret;
}

struct vfscore_file *vfscore_get_file(int fd)
{
	// unsigned long flags;
	struct vfscore_file *ret = NULL;

	show(fd);

	dump_fdtable();

	// flags = ukplat_lcpu_save_irqf();
	if (!uk_test_bit(fd, fdtable.bitmap)) {
		debug("test_bit not hit\n");
		show(uk_test_bit(fd, fdtable.bitmap));
		goto exit;
	}
	ret = fdtable.files[fd];
	fhold(ret);

	show(ret);

exit:
	// ukplat_lcpu_restore_irqf(flags);
	return ret;
}

int fget(int fd, struct vfscore_file **out_fp)
{
	int ret = 0;
	struct vfscore_file *fp = vfscore_get_file(fd);

	show(fd);
	show(fp);

	if (!fp)
		ret = EBADF;
	else
		*out_fp = fp;

	return ret;
}

int vfscore_put_fd(int fd)
{
	struct vfscore_file *fp;
	// unsigned long flags;

	/* FIXME Currently it is not allowed to free std(in|out|err):
	 * if (fd <= 2) return -EBUSY;
	 *
	 * However, returning -EBUSY in this case breaks dup2 with stdin, out,
	 * err. Ignoring this should be fine as long as those are not fdrop-ed
	 * twice, in which case the static fp would be freed, and here be
	 * dragons.
	 */

	// flags = ukplat_lcpu_save_irqf();
	uk_bitmap_clear(fdtable.bitmap, fd, 1);
	fp = fdtable.files[fd];
	fdtable.files[fd] = NULL;
	// ukplat_lcpu_restore_irqf(flags);

	/*
	 * Since we can alloc a fd without assigning a
	 * vfsfile we must protect against NULL ptr
	 */
	if (fp)
		fdrop(fp);

	return 0;
}

int vfscore_reserve_fd(int fd)
{
	int ret = 0;

	if (uk_test_bit(fd, fdtable.bitmap)) {
		ret = -EBUSY;
		goto exit;
	}

	uk_bitmap_set(fdtable.bitmap, fd, 1);

exit:
	return ret;
}

int dup3(int oldfd, int newfd, int flags)
{
	struct vfscore_file *fp, *fp_new;
	int error;

	/*
	 * Don't allow any argument but O_CLOEXEC.  But we even ignore
	 * that as we don't support exec() and thus don't care.
	 */
	if ((flags & ~O_CLOEXEC) != 0) {
		error = EINVAL;
		goto out_error;
	}

	if (oldfd == newfd) {
		error = EINVAL;
		goto out_error;
	}

	error = fget(oldfd, &fp);
	if (error)
		goto out_error;

	error = fget(newfd, &fp_new);
	if (error == 0) {
		/* if newfd is open, then close it */
		error = syscall_handler_close(newfd);
		if (error)
			goto out_error;
	}

	error = vfscore_reserve_fd(newfd);
	if (error)
		goto out_error;

	error = vfscore_install_fd(newfd, fp);
	if (error) {
		fdrop(fp);
		goto out_error;
	}

	fdrop(fp);
	return newfd;

out_error:
	if(error > 0)
		return -error;
	return error;
}

int dup2(int oldfd, int newfd)
{
	if (oldfd == newfd)
		return newfd;

	return dup3(oldfd, newfd, 0);
}

void fdtable_init(void)
{
	memset(&fdtable, 0, sizeof(fdtable));

	init_stdio();
}