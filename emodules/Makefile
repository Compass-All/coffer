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

clean:
	rm -rf $(build_dir)/emodules