#include "spi.h"
#include "drv_mmc.h"
#include "debug.h"
#include <stddef.h>

static void sifive_spi_prep_device(struct sifive_spi *spi,
				   struct dm_spi_slave_plat *slave_plat)
{
	/* Update the chip select polarity */
	if (slave_plat->mode & SPI_CS_HIGH)
		spi->cs_inactive &= ~BIT(slave_plat->cs);
	else
		spi->cs_inactive |= BIT(slave_plat->cs);
	writel(spi->cs_inactive, spi->regs + SIFIVE_SPI_REG_CSDEF);

	/* Select the correct device */
	writel(slave_plat->cs, spi->regs + SIFIVE_SPI_REG_CSID);
}

static int sifive_spi_set_cs(struct sifive_spi *spi,
			     struct dm_spi_slave_plat *slave_plat)
{
	u32 cs_mode = SIFIVE_SPI_CSMODE_MODE_HOLD;

	if (slave_plat->mode & SPI_CS_HIGH)
		cs_mode = SIFIVE_SPI_CSMODE_MODE_AUTO;

	writel(cs_mode, spi->regs + SIFIVE_SPI_REG_CSMODE);

	return 0;
}

static void sifive_spi_clear_cs(struct sifive_spi *spi)
{
	writel(SIFIVE_SPI_CSMODE_MODE_AUTO, spi->regs + SIFIVE_SPI_REG_CSMODE);
}

static void sifive_spi_prep_transfer(struct sifive_spi *spi,
				     struct dm_spi_slave_plat *slave_plat,
				     u8 *rx_ptr)
{
	u32 cr;

	/* Modify the SPI protocol mode */
	cr = readl(spi->regs + SIFIVE_SPI_REG_FMT);

	/* Bits per word ? */
	cr &= ~SIFIVE_SPI_FMT_LEN_MASK;
	cr |= SIFIVE_SPI_FMT_LEN(spi->bits_per_word);

	/* LSB first? */
	cr &= ~SIFIVE_SPI_FMT_ENDIAN;
	if (slave_plat->mode & SPI_LSB_FIRST)
		cr |= SIFIVE_SPI_FMT_ENDIAN;

	/* Number of wires ? */
	cr &= ~SIFIVE_SPI_FMT_PROTO_MASK;
	switch (spi->fmt_proto) {
	case SIFIVE_SPI_PROTO_QUAD:
		cr |= SIFIVE_SPI_FMT_PROTO_QUAD;
		break;
	case SIFIVE_SPI_PROTO_DUAL:
		cr |= SIFIVE_SPI_FMT_PROTO_DUAL;
		break;
	default:
		cr |= SIFIVE_SPI_FMT_PROTO_SINGLE;
		break;
	}

	/* SPI direction in/out ? */
	cr &= ~SIFIVE_SPI_FMT_DIR;
	if (!rx_ptr)
		cr |= SIFIVE_SPI_FMT_DIR;

	writel(cr, spi->regs + SIFIVE_SPI_REG_FMT);
}

static void sifive_spi_rx(struct sifive_spi *spi, u8 *rx_ptr)
{
	u32 data;

	do {
		data = readl(spi->regs + SIFIVE_SPI_REG_RXDATA);
	} while (data & SIFIVE_SPI_RXDATA_EMPTY);

	if (rx_ptr)
		*rx_ptr = data & SIFIVE_SPI_RXDATA_DATA_MASK;
}

static void sifive_spi_tx(struct sifive_spi *spi, const u8 *tx_ptr)
{
	u32 data;
	u8 tx_data = (tx_ptr) ? *tx_ptr & SIFIVE_SPI_TXDATA_DATA_MASK :
				SIFIVE_SPI_TXDATA_DATA_MASK;

	do {
		data = readl(spi->regs + SIFIVE_SPI_REG_TXDATA);
	} while (data & SIFIVE_SPI_TXDATA_FULL);

	writel(tx_data, spi->regs + SIFIVE_SPI_REG_TXDATA);
}

static int sifive_spi_wait(struct sifive_spi *spi, u32 bit)
{
	return wait_for_bit_le32(spi->regs + SIFIVE_SPI_REG_IP,
				 bit, true, 100, false);
}

#define min(x, y) ({				\
	typeof(x) _min1 = (x);			\
	typeof(y) _min2 = (y);			\
	(void) (&_min1 == &_min2);		\
	_min1 < _min2 ? _min1 : _min2; })

int sifive_spi_xfer(struct udevice *dev, unsigned int bitlen,
			   const void *dout, void *din, unsigned long flags)
{
	// struct udevice *bus = dev->parent;
	// struct sifive_spi *spi = dev_get_priv(bus);
	// struct dm_spi_slave_plat *slave_plat = dev_get_parent_plat(dev);
	struct sifive_spi *spi = dev->spi;
	struct dm_spi_slave_plat *slave_plat = dev->slave_plat;
	
	debug("############# flag1 ############\n");


	const u8 *tx_ptr = dout;
	u8 *rx_ptr = din;
	u32 remaining_len;
	int ret;

	if (flags & SPI_XFER_BEGIN) {
		sifive_spi_prep_device(spi, slave_plat);

		ret = sifive_spi_set_cs(spi, slave_plat);
		if (ret)
			return ret;
	}

	debug("############# flag2 ############\n");

	sifive_spi_prep_transfer(spi, slave_plat, rx_ptr);

	debug("############# flag3 ############\n");

	remaining_len = bitlen / 8;

	while (remaining_len) {
		unsigned int n_words = min(remaining_len, spi->fifo_depth);
		unsigned int tx_words, rx_words;

		/* Enqueue n_words for transmission */
		for (tx_words = 0; tx_words < n_words; tx_words++) {
			if (!tx_ptr)
				sifive_spi_tx(spi, NULL);
			else
				sifive_spi_tx(spi, tx_ptr++);
		}

		if (rx_ptr) {
			/* Wait for transmission + reception to complete */
			writel(n_words - 1, spi->regs + SIFIVE_SPI_REG_RXMARK);
			ret = sifive_spi_wait(spi, SIFIVE_SPI_IP_RXWM);
			if (ret)
				return ret;

			/* Read out all the data from the RX FIFO */
			for (rx_words = 0; rx_words < n_words; rx_words++)
				sifive_spi_rx(spi, rx_ptr++);
		} else {
			/* Wait for transmission to complete */
			ret = sifive_spi_wait(spi, SIFIVE_SPI_IP_TXWM);
			if (ret)
				return ret;
		}

		remaining_len -= n_words;
	}

	debug("############# flag4 ############\n");

	if (flags & SPI_XFER_END)
		sifive_spi_clear_cs(spi);

	debug("############# flag5 ############\n");

	return 0;
}