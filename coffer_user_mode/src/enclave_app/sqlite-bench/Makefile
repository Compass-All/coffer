CC=riscv64-linux-musl-gcc
CFLAGS=-Wall -I. -O2 -DNDEBUG -std=c99 -g
LDFLAGS=-lpthread -ldl -lm -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
HDRS=$(wildcard *.h)
TARGET=sqlite-bench
BUILDDIR = ../../../../build/prog/enclave/$(shell basename $(CURDIR))

# all: $(TARGET) copy
all:
	echo "skip sqlite"

$(TARGET): $(OBJS)
	$(CC) -DTHREADSAFE=1 -o $@ $^ $(LDFLAGS)

$(OBJS): $(HDRS)

bench: $(TARGET) clean-db
	./$(TARGET)

copy: $(TARGET)
	mkdir -p $(BUILDDIR)
	cp $(TARGET) $(BUILDDIR)

clean:
	rm -f $(TARGET) *.o

clean-db:
	rm -f dbbench_sqlite3*

.PHONY: all bench clean clean-db
