#pragma once

/**
 * struct spi_slave - Representation of a SPI slave
 *
 * For driver model this is the per-child data used by the SPI bus. It can
 * be accessed using dev_get_parent_priv() on the slave device. The SPI uclass
 * sets up per_child_auto to sizeof(struct spi_slave), and the
 * driver should not override it. Two platform data fields (max_hz and mode)
 * are copied into this structure to provide an initial value. This allows
 * them to be changed, since we should never change platform data in drivers.
 *
 * If not using driver model, drivers are expected to extend this with
 * controller-specific data.
 *
 * @dev:		SPI slave device
 * @max_hz:		Maximum speed for this slave
 * @bus:		ID of the bus that the slave is attached to. For
 *			driver model this is the sequence number of the SPI
 *			bus (dev_seq(bus)) so does not need to be stored
 * @cs:			ID of the chip select connected to the slave.
 * @mode:		SPI mode to use for this slave (see SPI mode flags)
 * @wordlen:		Size of SPI word in number of bits
 * @max_read_size:	If non-zero, the maximum number of bytes which can
 *			be read at once.
 * @max_write_size:	If non-zero, the maximum number of bytes which can
 *			be written at once.
 * @memory_map:		Address of read-only SPI flash access.
 * @flags:		Indication of SPI flags.
 */
struct spi_slave {
	uint max_hz;
	uint mode;
	unsigned int wordlen;
	unsigned int max_read_size;
	unsigned int max_write_size;
	void *memory_map;

	u8 flags;
#define SPI_XFER_BEGIN		BIT(0)	/* Assert CS before transfer */
#define SPI_XFER_END		BIT(1)	/* Deassert CS after transfer */
#define SPI_XFER_ONCE		(SPI_XFER_BEGIN | SPI_XFER_END)
};
