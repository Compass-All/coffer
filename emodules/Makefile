subdirs = $(patsubst %/,%,$(wildcard */))

ifdef O
 build_dir=$(shell $(READLINK) -f $(O))
else
 build_dir=$(CURDIR)/../build
endif

.PHONY: all $(subdirs) clean

all: dir $(subdirs)

dir:
	mkdir -p $(build_dir)

$(subdirs):
	make -C $@ \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		TARGET_PLATFORM=$(TARGET_PLATFORM) \
		DEBUG=$(DEBUG)

clean:
	rm -rf $(build_dir)/emodules