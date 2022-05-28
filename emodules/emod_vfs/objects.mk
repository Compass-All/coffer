emodule-objs-y = 
emodule-objs-y += emod_vfs.o 
emodule-objs-y += dependency.o 
emodule-objs-y += vfs/syscall_handlers.o 
emodule-objs-y += vfs/syscalls.o
emodule-objs-y += vfs/fd.o
emodule-objs-y += vfs/file.o
emodule-objs-y += vfs/fops.o
emodule-objs-y += vfs/vnode.o
emodule-objs-y += vfs/dentry.o
emodule-objs-y += vfs/lookup.o
emodule-objs-y += vfs/task.o 
emodule-objs-y += vfs/string.o 
emodule-objs-y += vfs/errno.o 
emodule-objs-y += vfs/mount.o 
emodule-objs-y += vfs/stdio.o
emodule-objs-y += vfs/rootfs.o
emodule-objs-y += ramfs/ramfs_vfsops.o
emodule-objs-y += ramfs/ramfs_vnops.o
emodule-objs-y += mmap/mmap.o

emodule-genflags = 
emodule-genflags += -DMMAP_START_VA=$(MMAP_START_VA)