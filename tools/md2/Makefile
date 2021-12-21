target_dir := build

CC := gcc
FLAGS := -O2

src := $(wildcard *.c)
targets := $(src:.c=)

all: dir $(addprefix $(target_dir)/, $(targets))

dir:
	mkdir -p $(target_dir)

$(target_dir)/%: %.c
	$(CC) $< -o $@ $(FLAGS)

clean:
	-@rm -rf $(target_dir)
	-@rm -rf logfile