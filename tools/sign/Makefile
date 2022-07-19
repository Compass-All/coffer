BUILD_DIR = ../../build/tools

all: dirs key_gen sign

dirs:
	@mkdir -p $(BUILD_DIR)

key_gen: key_gen.c ecc.c ecc.h md2.c md2.h
	gcc key_gen.c ecc.c md2.c -O2 -o $(BUILD_DIR)/$@

sign: sign.c ecc.c ecc.h md2.c md2.h
	gcc sign.c ecc.c md2.c -O2 -o $(BUILD_DIR)/$@

clean:
	-@rm $(BUILD_DIR)/key_gen $(BUILD_DIR)/sign