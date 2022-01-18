#pragma once
#include <driver/driver.h>

// dummy udevice
struct udevice {
	struct sifive_spi *spi;
	struct dm_spi_slave_plat *slave_plat;
};

int reg_map_setup(volatile extra_module_t *emod);