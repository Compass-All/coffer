name = $(notdir $(shell pwd))
target = riscv64imac-unknown-none-elf
version = debug
objcopy = riscv64-unknown-elf-objcopy
objdump = riscv64-unknown-elf-objdump
build_dir = $(CURDIR)/../../build
target_dir = $(build_dir)/emodules/$(name)

all: $(target_dir) $(target_dir)/$(name).bin

$(target_dir)/$(name).bin: target/$(target)/$(version)/$(name).bin
	cp $< $@

target/$(target)/$(version)/$(name).bin: target/$(target)/$(version)/$(name)
	$(objcopy) -O binary --set-section-flags .bss=alloc,load,contents $< $@

target/$(target)/$(version)/$(name):
	-cargo build
	chown -R 1000:1000 -R target
	cp $@ $(target_dir)/$(name).elf
	$(objdump) -d $(target_dir)/$(name).elf > $(target_dir)/$(name).asm

$(target_dir):
	mkdir -p $(target_dir)

clean:
	cargo clean
	rm -rf $(target_dir)

.PHONY: all clean
