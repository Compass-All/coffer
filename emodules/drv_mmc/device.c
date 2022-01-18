#include "device.h"
#include <driver/driver.h>
#include "debug.h"
#include "spi.h"

struct udevice mmc_sd;
struct sifive_spi spi;

void dump_reg(char *name, u64 offset)
{
	u32 reg = readl(spi.regs + offset);
	debug("%s = 0x%lx\n", name, reg);
}

void test()
{
	dump_reg("sckdiv", 0);
	dump_reg("sckmode", 0x4);
	dump_reg("csid", 0x10);
	dump_reg("csdef", 0x14);
	dump_reg("csmode", 0x18);
}

int reg_map_setup(volatile extra_module_t *emod)
{
	if (!emod) {
		debug("error\n");
		return -1;
	}

	uintptr_t base = emod->peripheral.peri_region[0].start;

	spi.regs = (void *)base;

	test();
	
	return 0;
}