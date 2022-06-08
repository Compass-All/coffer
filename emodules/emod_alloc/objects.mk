emodule-objs-y = 
emodule-objs-y += emod_alloc.o 
emodule-objs-y += llist.o 
emodule-objs-y += heap.o 
emodule-objs-y += dependency.o 

emodule-genflags = -DPOOL_VA_START=$(POOL_VA_START)