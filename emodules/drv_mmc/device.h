#pragma once
#include <driver/driver.h>

// dummy udevice
struct udevice {
	struct sifive_spi *spi;
	struct dm_spi_slave_plat *slave_plat;
};

extern struct udevice mmc_sd;

int reg_map_setup(volatile extra_module_t *emod);