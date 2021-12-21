subdirs = $(patsubst %/,%,$(wildcard */))

ifdef O
 build_dir=$(shell $(READLINK) -f $(O))
else
 build_dir=$(CURDIR)/../build
endif

.PHONY: all $(subdirs) clean

all: $(subdirs)

$(subdirs):
	make -C $@ CROSS_COMPILE=$(CROSS_COMPILE)
	../tools/md2/build/md2 ../build/emodules/$@/$@.bin > ../build/emodules/$@/$@.md2

clean:
	rm -rf $(build_dir)/emodules