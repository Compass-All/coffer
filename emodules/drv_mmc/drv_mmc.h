#pragma once

#include "spi.h"

#define SPI_MODE_0				(0|0)			/* (original MicroWire) */
#define MMC_MODE_SPI		BIT(27)

#define MMC_VDD_32_33		0x00100000	/* VDD voltage 3.2 ~ 3.3 */
#define MMC_VDD_33_34		0x00200000	/* VDD voltage 3.3 ~ 3.4 */

#define MMC_SPI_MAX_CLOCK		25000000 		/* SD/MMC legacy speed */
#define MMC_SPI_MIN_CLOCK		400000	/* 400KHz to meet MMC spec */

#define MMC_SPI_VOLTAGE			(MMC_VDD_32_33 | MMC_VDD_33_34)

#define PART_TYPE_DOS		0x02

#define CONFIG_SYS_MMC_MAX_BLK_COUNT 65535

#define MMC_STATUS_MASK		(~0x0206BF7F)
#define MMC_STATUS_SWITCH_ERROR	(1 << 7)
#define MMC_STATUS_RDY_FOR_DATA (1 << 8)
#define MMC_STATUS_CURR_STATE	(0xf << 9)
#define MMC_STATUS_ERROR	(1 << 19)

#define MMC_DATA_READ		1
#define MMC_DATA_WRITE		2

#define MMC_SPI_CMD(x) (0x40 | (x))

#define SD_CMD_APP_SEND_OP_COND			41
#define MMC_CMD_SEND_OP_COND			1
#define SD_CMD_SEND_IF_COND				8
#define MMC_CMD_SPI_READ_OCR			58
#define MMC_CMD_SEND_STATUS				13
#define MMC_CMD_SET_BLOCKLEN			16
#define MMC_CMD_SPI_CRC_ON_OFF			59
#define MMC_CMD_STOP_TRANSMISSION		12
#define MMC_CMD_ERASE					38
#define MMC_CMD_SEND_CSD				9
#define MMC_CMD_SEND_CID				10
#define MMC_CMD_READ_SINGLE_BLOCK		17
#define MMC_CMD_READ_MULTIPLE_BLOCK		18
#define MMC_CMD_WRITE_SINGLE_BLOCK		24
#define MMC_CMD_WRITE_MULTIPLE_BLOCK	25
#define MMC_CMD_APP_CMD					55
#define SD_CMD_ERASE_WR_BLK_START		32
#define SD_CMD_ERASE_WR_BLK_END			33

/* MMC/SD in SPI mode reports R1 status always */
#define R1_SPI_IDLE			BIT(0)
#define R1_SPI_ERASE_RESET		BIT(1)
#define R1_SPI_ILLEGAL_COMMAND		BIT(2)
#define R1_SPI_COM_CRC			BIT(3)
#define R1_SPI_ERASE_SEQ		BIT(4)
#define R1_SPI_ADDRESS			BIT(5)
#define R1_SPI_PARAMETER		BIT(6)
/* R1 bit 7 is always zero, reuse this bit for error */
#define R1_SPI_ERROR			BIT(7)

#define MMC_RSP_PRESENT (1 << 0)
#define MMC_RSP_136	(1 << 1)		/* 136 bit response */
#define MMC_RSP_CRC	(1 << 2)		/* expect valid crc */
#define MMC_RSP_BUSY	(1 << 3)		/* card may send busy */
#define MMC_RSP_OPCODE	(1 << 4)		/* response contains opcode */

#define MMC_RSP_NONE	(0)
#define MMC_RSP_R1	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R1b	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE| \
			MMC_RSP_BUSY)
#define MMC_RSP_R2	(MMC_RSP_PRESENT|MMC_RSP_136|MMC_RSP_CRC)
#define MMC_RSP_R3	(MMC_RSP_PRESENT)
#define MMC_RSP_R4	(MMC_RSP_PRESENT)
#define MMC_RSP_R5	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R6	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)
#define MMC_RSP_R7	(MMC_RSP_PRESENT|MMC_RSP_CRC|MMC_RSP_OPCODE)

/*
 * Read and write blocks start with these tokens and end with crc;
 * on error, read tokens act like a subset of R2_SPI_* values.
 */
/* single block write multiblock read */
#define SPI_TOKEN_SINGLE		0xfe
/* multiblock write */
#define SPI_TOKEN_MULTI_WRITE		0xfc
/* terminate multiblock write */
#define SPI_TOKEN_STOP_TRAN		0xfd

/* Response tokens used to ack each block written: */
#define SPI_MMC_RESPONSE_CODE(x)	((x) & 0x1f)
#define SPI_RESPONSE_ACCEPTED		((2 << 1)|1)
#define SPI_RESPONSE_CRC_ERR		((5 << 1)|1)
#define SPI_RESPONSE_WRITE_ERR		((6 << 1)|1)

#define OCR_BUSY		0x80000000
#define OCR_HCS			0x40000000
#define OCR_S18R		0x1000000
#define OCR_VOLTAGE_MASK	0x007FFF80
#define OCR_ACCESS_MODE		0x60000000

/* timeout value */
#define CMD_TIMEOUT			8
#define READ_TIMEOUT			3000000 /* 1 sec */
#define WRITE_TIMEOUT			3000000 /* 1 sec */
#define R1B_TIMEOUT			3000000 /* 1 sec */

// errors
#define	ECOMM		70	/* Communication error on send */
#define	ETIMEDOUT	110	/* Connection timed out */

#define swab16(x) ((u16)(             \
    (((u16)(x) & (u16)0x00ffU) << 8) |          \
    (((u16)(x) & (u16)0xff00U) >> 8)))

#define swab32(x) ((u32)(             \
    (((u32)(x) & (u32)0x000000ffUL) << 24) |        \
    (((u32)(x) & (u32)0x0000ff00UL) <<  8) |        \
    (((u32)(x) & (u32)0x00ff0000UL) >>  8) |        \
    (((u32)(x) & (u32)0xff000000UL) >> 24)))

#define swab64(x) ((u64)(             \
    (((u64)(x) & (u64)0x00000000000000ffULL) << 56) |   \
    (((u64)(x) & (u64)0x000000000000ff00ULL) << 40) |   \
    (((u64)(x) & (u64)0x0000000000ff0000ULL) << 24) |   \
    (((u64)(x) & (u64)0x00000000ff000000ULL) <<  8) |   \
    (((u64)(x) & (u64)0x000000ff00000000ULL) >>  8) |   \
    (((u64)(x) & (u64)0x0000ff0000000000ULL) >> 24) |   \
    (((u64)(x) & (u64)0x00ff000000000000ULL) >> 40) |   \
    (((u64)(x) & (u64)0xff00000000000000ULL) >> 56)))

#define cpu_to_be64(x) ((u64)swab64((x)))
#define cpu_to_be32(x) ((u32)swab32((x)))
#define cpu_to_be16(x) ((u16)swab16((x)))
#define be64_to_cpu(x) swab64((u64)(x))
#define be32_to_cpu(x) swab32((u32)(x))
#define be16_to_cpu(x) swab16((u16)(x))

struct mmc_cmd {
	ushort cmdidx;
	uint resp_type;
	uint cmdarg;
	uint response[4];
};

struct mmc_data {
	union {
		char *dest;
		const char *src; /* src buffers don't get written to */
	};
	uint flags;
	uint blocks;
	uint blocksize;
};

// struct mmc_config {
// 	const char *name;
// 	const struct mmc_ops *ops;
// 	uint host_caps;
// 	uint voltages;
// 	uint f_min;
// 	uint f_max;
// 	uint b_max;
// 	unsigned char part_type;
// 	struct udevice *pwr_dev;
// };

// struct mmc {
// 	const struct mmc_config *cfg;	/* provided configuration */
// };

extern const u8 crc7_syndrome_table[256];
static inline u8 crc7_byte(u8 crc, u8 data)
{
	return crc7_syndrome_table[(crc << 1) ^ data];
}

static inline int dm_spi_xfer(struct udevice *dev, unsigned int bitlen,
			   const void *dout, void *din, unsigned long flags)
{
	int ret = 0;

	debug("############ flag1\n");

	ret = sifive_spi_xfer(dev, bitlen, dout, din, flags);

	debug("############ flag2\n");

	return ret;
}