subdirs = $(patsubst %/,%,$(wildcard */))

ifdef O
 build_dir=$(shell $(READLINK) -f $(O))
else
 build_dir=$(CURDIR)/../build
endif

SIGN_UTIL_PATH = $(build_dir)/tools/sign

.PHONY: all $(subdirs) clean

all: dir $(subdirs)

dir:
	mkdir -p $(build_dir)

$(subdirs):
	make -C $@ \
		CROSS_COMPILE=$(CROSS_COMPILE) \
		TARGET_PLATFORM=$(TARGET_PLATFORM) \
		DEBUG=$(DEBUG) \
	&& $(SIGN_UTIL_PATH) $(build_dir)/emodules/$@/$@.bin \
		> $(build_dir)/emodules/$@/$@.sign \
	&& echo "signature: " \
	&& cat $(build_dir)/emodules/$@/$@.sign \
	&& cp $(build_dir)/emodules/$@/$@.bin $(build_dir)/emodules/$@/$@.bin.signed \
	&& cat $(build_dir)/emodules/$@/$@.sign | \
		xxd -r -p >> $(build_dir)/emodules/$@/$@.bin.signed

clean:
	rm -rf $(build_dir)/emodules