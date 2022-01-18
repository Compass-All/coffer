// See LICENSE for license details.

// #include <string.h>
// #include <stdarg.h>
// #include <assert.h>
#include "drv_mmc.h"
#include "spi.h"
#include <stdint.h>
#include <driver/driver.h>
#include "debug.h"
#include "device.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>


/* Table for CRC-7 (polynomial x^7 + x^3 + 1) */
const u8 crc7_syndrome_table[256] = {
	0x00, 0x09, 0x12, 0x1b, 0x24, 0x2d, 0x36, 0x3f,
	0x48, 0x41, 0x5a, 0x53, 0x6c, 0x65, 0x7e, 0x77,
	0x19, 0x10, 0x0b, 0x02, 0x3d, 0x34, 0x2f, 0x26,
	0x51, 0x58, 0x43, 0x4a, 0x75, 0x7c, 0x67, 0x6e,
	0x32, 0x3b, 0x20, 0x29, 0x16, 0x1f, 0x04, 0x0d,
	0x7a, 0x73, 0x68, 0x61, 0x5e, 0x57, 0x4c, 0x45,
	0x2b, 0x22, 0x39, 0x30, 0x0f, 0x06, 0x1d, 0x14,
	0x63, 0x6a, 0x71, 0x78, 0x47, 0x4e, 0x55, 0x5c,
	0x64, 0x6d, 0x76, 0x7f, 0x40, 0x49, 0x52, 0x5b,
	0x2c, 0x25, 0x3e, 0x37, 0x08, 0x01, 0x1a, 0x13,
	0x7d, 0x74, 0x6f, 0x66, 0x59, 0x50, 0x4b, 0x42,
	0x35, 0x3c, 0x27, 0x2e, 0x11, 0x18, 0x03, 0x0a,
	0x56, 0x5f, 0x44, 0x4d, 0x72, 0x7b, 0x60, 0x69,
	0x1e, 0x17, 0x0c, 0x05, 0x3a, 0x33, 0x28, 0x21,
	0x4f, 0x46, 0x5d, 0x54, 0x6b, 0x62, 0x79, 0x70,
	0x07, 0x0e, 0x15, 0x1c, 0x23, 0x2a, 0x31, 0x38,
	0x41, 0x48, 0x53, 0x5a, 0x65, 0x6c, 0x77, 0x7e,
	0x09, 0x00, 0x1b, 0x12, 0x2d, 0x24, 0x3f, 0x36,
	0x58, 0x51, 0x4a, 0x43, 0x7c, 0x75, 0x6e, 0x67,
	0x10, 0x19, 0x02, 0x0b, 0x34, 0x3d, 0x26, 0x2f,
	0x73, 0x7a, 0x61, 0x68, 0x57, 0x5e, 0x45, 0x4c,
	0x3b, 0x32, 0x29, 0x20, 0x1f, 0x16, 0x0d, 0x04,
	0x6a, 0x63, 0x78, 0x71, 0x4e, 0x47, 0x5c, 0x55,
	0x22, 0x2b, 0x30, 0x39, 0x06, 0x0f, 0x14, 0x1d,
	0x25, 0x2c, 0x37, 0x3e, 0x01, 0x08, 0x13, 0x1a,
	0x6d, 0x64, 0x7f, 0x76, 0x49, 0x40, 0x5b, 0x52,
	0x3c, 0x35, 0x2e, 0x27, 0x18, 0x11, 0x0a, 0x03,
	0x74, 0x7d, 0x66, 0x6f, 0x50, 0x59, 0x42, 0x4b,
	0x17, 0x1e, 0x05, 0x0c, 0x33, 0x3a, 0x21, 0x28,
	0x5f, 0x56, 0x4d, 0x44, 0x7b, 0x72, 0x69, 0x60,
	0x0e, 0x07, 0x1c, 0x15, 0x2a, 0x23, 0x38, 0x31,
	0x46, 0x4f, 0x54, 0x5d, 0x62, 0x6b, 0x70, 0x79
};


/* Table of CRC constants - implements x^16+x^12+x^5+1 */
static const uint16_t crc16_tab[] = {
	0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
	0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
	0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
	0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
	0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
	0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
	0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
	0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
	0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
	0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
	0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
	0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
	0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
	0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
	0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
	0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
	0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
	0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
	0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
	0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
	0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
	0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
	0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
	0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
	0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
	0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
	0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
	0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
	0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
	0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
	0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
	0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0,
};

// ------------------------------
__attribute__((unused))
static int mmc_spi_probe (
	struct spi_slave *spi,
	struct mmc_config *cfg,
	struct mmc *mmc
)
{
	char name[] = "mmc-spi";

	if (!spi->max_hz)
		spi->max_hz = MMC_SPI_MAX_CLOCK;
	spi->mode = SPI_MODE_0;
	spi->wordlen = 8;

	cfg->name = name;
	cfg->host_caps = MMC_MODE_SPI;
	cfg->voltages = MMC_SPI_VOLTAGE;
	cfg->f_min = MMC_SPI_MIN_CLOCK;
	cfg->f_max = spi->max_hz;
	cfg->part_type = PART_TYPE_DOS;
	cfg->b_max = CONFIG_SYS_MMC_MAX_BLK_COUNT;

	mmc->cfg = cfg;

	return 0;
}

u8 crc7(u8 crc, const u8 *buffer, size_t len)
{
	while (len--)
		crc = crc7_byte(crc, *buffer++);
	return crc;
}

uint16_t crc16_ccitt(uint16_t cksum, const unsigned char *buf, int len)
{
	for (int i = 0;  i < len;  i++)
		cksum = crc16_tab[((cksum>>8) ^ *buf++) & 0xff] ^ (cksum << 8);

	return cksum;
}

/**
 * mmc_spi_sendcmd() - send a command to the SD card
 *
 * @dev:	mmc_spi device
 * @cmdidx:	command index
 * @cmdarg:	command argument
 * @resp_type:	card response type
 * @resp:	buffer to store the card response
 * @resp_size:	size of the card response
 * @resp_match:	if true, compare each of received bytes with @resp_match_value
 * @resp_match_value:	a value to be compared with each of received bytes
 * @r1b:	if true, receive additional bytes for busy signal token
 * @return 0 if OK, -ETIMEDOUT if no card response is received, -ve on error
 * 
 */
static int mmc_spi_sendcmd (
	struct udevice *dev,
	ushort cmdidx, u32 cmdarg, u32 resp_type,
	u8 *resp, u32 resp_size,
	bool resp_match, u8 resp_match_value, bool r1b
)
{
	int i, rpos = 0, ret = 0;
	u8 cmdo[7], r;

	if (!resp || !resp_size)
		return 0;

	debug("%s: cmd%d cmdarg=0x%x resp_type=0x%x "
	      "resp_size=%d resp_match=%d resp_match_value=0x%x\n",
	      __func__, cmdidx, cmdarg, resp_type,
	      resp_size, resp_match, resp_match_value);

	cmdo[0] = 0xff;
	cmdo[1] = MMC_SPI_CMD(cmdidx);
	cmdo[2] = cmdarg >> 24;
	cmdo[3] = cmdarg >> 16;
	cmdo[4] = cmdarg >> 8;
	cmdo[5] = cmdarg;
	cmdo[6] = (crc7(0, &cmdo[1], 5) << 1) | 0x01;
	ret = dm_spi_xfer(dev, sizeof(cmdo) * 8, cmdo, NULL, SPI_XFER_BEGIN);
	if (ret)
		return ret;

	ret = dm_spi_xfer(dev, 1 * 8, NULL, &r, 0);
	if (ret)
		return ret;

	debug("%s: cmd%d", __func__, cmdidx);

	if (resp_match)
		r = ~resp_match_value;
	i = CMD_TIMEOUT;
	while (i) {
		ret = dm_spi_xfer(dev, 1 * 8, NULL, &r, 0);
		if (ret)
			return ret;
		debug(" resp%d=0x%x", rpos, r);
		rpos++;
		i--;

		if (resp_match) {
			if (r == resp_match_value)
				break;
		} else {
			if (!(r & 0x80))
				break;
		}

		if (!i)
			return -ETIMEDOUT;
	}

	resp[0] = r;
	for (i = 1; i < resp_size; i++) {
		ret = dm_spi_xfer(dev, 1 * 8, NULL, &r, 0);
		if (ret)
			return ret;
		debug(" resp%d=0x%x", rpos, r);
		rpos++;
		resp[i] = r;
	}

	if (r1b == true) {
		i = R1B_TIMEOUT;
		while (i) {
			ret = dm_spi_xfer(dev, 1 * 8, NULL, &r, 0);
			if (ret)
				return ret;

			debug(" resp%d=0x%x", rpos, r);
			rpos++;
			i--;

			if (r)
				break;
		}
		if (!i)
			return -ETIMEDOUT;
	}

	debug("\n");

	return 0;
}

/**
 * mmc_spi_readdata() - read data block(s) from the SD card
 *
 * @dev:	mmc_spi device
 * @xbuf:	buffer of the actual data (excluding token and crc) to read
 * @bcnt:	number of data blocks to transfer
 * @bsize:	size of the actual data (excluding token and crc) in bytes
 * @return 0 if OK, -ECOMM if crc error, -ETIMEDOUT on other errors
 * 
 */
static int mmc_spi_readdata (
	struct udevice *dev,
	void *xbuf, u32 bcnt, u32 bsize
)
{
	u16 crc;
	u8 *buf = xbuf, r1;
	int i, ret = 0;

	while (bcnt--) {
		for (i = 0; i < READ_TIMEOUT; i++) {
			ret = dm_spi_xfer(dev, 1 * 8, NULL, &r1, 0);
			if (ret)
				return ret;
			if (r1 == SPI_TOKEN_SINGLE)
				break;
		}
		debug("%s: data tok%d 0x%x\n", __func__, i, r1);
		if (r1 == SPI_TOKEN_SINGLE) {
			ret = dm_spi_xfer(dev, bsize * 8, NULL, buf, 0);
			if (ret)
				return ret;
			ret = dm_spi_xfer(dev, 2 * 8, NULL, &crc, 0);
			if (ret)
				return ret;
#ifdef CONFIG_MMC_SPI_CRC_ON
			u16 crc_ok = be16_to_cpu(crc16_ccitt(0, buf, bsize));
			if (crc_ok != crc) {
				debug("%s: data crc error, expected %04x got %04x\n",
				      __func__, crc_ok, crc);
				r1 = R1_SPI_COM_CRC;
				break;
			}
#endif
			r1 = 0;
		} else {
			r1 = R1_SPI_ERROR;
			break;
		}
		buf += bsize;
	}

	if (r1 & R1_SPI_COM_CRC)
		ret = -ECOMM;
	else if (r1) /* other errors */
		ret = -ETIMEDOUT;

	return ret;
}

/**
 * mmc_spi_writedata() - write data block(s) to the SD card
 *
 * @dev:	mmc_spi device
 * @xbuf:	buffer of the actual data (excluding token and crc) to write
 * @bcnt:	number of data blocks to transfer
 * @bsize:	size of actual data (excluding token and crc) in bytes
 * @multi:	indicate a transfer by multiple block write command (CMD25)
 * @return 0 if OK, -ECOMM if crc error, -ETIMEDOUT on other errors
 * 
 */
static int mmc_spi_writedata(struct udevice *dev, const void *xbuf,
			     u32 bcnt, u32 bsize, int multi)
{
	const u8 *buf = xbuf;
	u8 r1, tok[2];
	u16 crc;
	int i, ret = 0;

	tok[0] = 0xff;
	tok[1] = multi ? SPI_TOKEN_MULTI_WRITE : SPI_TOKEN_SINGLE;

	while (bcnt--) {
#ifdef CONFIG_MMC_SPI_CRC_ON
		crc = cpu_to_be16(crc16_ccitt(0, (u8 *)buf, bsize));
#endif
		dm_spi_xfer(dev, 2 * 8, tok, NULL, 0);
		dm_spi_xfer(dev, bsize * 8, buf, NULL, 0);
		dm_spi_xfer(dev, 2 * 8, &crc, NULL, 0);
		for (i = 0; i < CMD_TIMEOUT; i++) {
			dm_spi_xfer(dev, 1 * 8, NULL, &r1, 0);
			if ((r1 & 0x10) == 0) /* response token */
				break;
		}
		debug("%s: data tok%d 0x%x\n", __func__, i, r1);
		if (SPI_MMC_RESPONSE_CODE(r1) == SPI_RESPONSE_ACCEPTED) {
			debug("%s: data accepted\n", __func__);
			for (i = 0; i < WRITE_TIMEOUT; i++) { /* wait busy */
				dm_spi_xfer(dev, 1 * 8, NULL, &r1, 0);
				if (i && r1 == 0xff) {
					r1 = 0;
					break;
				}
			}
			if (i == WRITE_TIMEOUT) {
				debug("%s: data write timeout 0x%x\n",
				      __func__, r1);
				r1 = R1_SPI_ERROR;
				break;
			}
		} else {
			debug("%s: data error 0x%x\n", __func__, r1);
			r1 = R1_SPI_COM_CRC;
			break;
		}
		buf += bsize;
	}
	if (multi && bcnt == -1) { /* stop multi write */
		tok[1] = SPI_TOKEN_STOP_TRAN;
		dm_spi_xfer(dev, 2 * 8, tok, NULL, 0);
		for (i = 0; i < WRITE_TIMEOUT; i++) { /* wait busy */
			dm_spi_xfer(dev, 1 * 8, NULL, &r1, 0);
			if (i && r1 == 0xff) {
				r1 = 0;
				break;
			}
		}
		if (i == WRITE_TIMEOUT) {
			debug("%s: data write timeout 0x%x\n", __func__, r1);
			r1 = R1_SPI_ERROR;
		}
	}

	if (r1 & R1_SPI_COM_CRC)
		ret = -ECOMM;
	else if (r1) /* other errors */
		ret = -ETIMEDOUT;

	return ret;
}

__attribute__((unused))
static int dm_mmc_spi_request (
	struct udevice *dev,
	struct mmc_cmd *cmd,
	struct mmc_data *data
)
{
	int i, multi, ret = 0;
	u8 *resp = NULL;
	u32 resp_size = 0;
	bool resp_match = false, r1b = false;
	u8 resp8 = 0, resp16[2] = { 0 }, resp40[5] = { 0 }, resp_match_value = 0;

	for (i = 0; i < 4; i++)
		cmd->response[i] = 0;

	switch (cmd->cmdidx) {
	case SD_CMD_APP_SEND_OP_COND:
	case MMC_CMD_SEND_OP_COND:
		resp = &resp8;
		resp_size = sizeof(resp8);
		cmd->cmdarg = 0x40000000;
		break;
	case SD_CMD_SEND_IF_COND:
		resp = (u8 *)&resp40[0];
		resp_size = sizeof(resp40);
		resp_match = true;
		resp_match_value = R1_SPI_IDLE;
		break;
	case MMC_CMD_SPI_READ_OCR:
		resp = (u8 *)&resp40[0];
		resp_size = sizeof(resp40);
		break;
	case MMC_CMD_SEND_STATUS:
		resp = (u8 *)&resp16[0];
		resp_size = sizeof(resp16);
		break;
	case MMC_CMD_SET_BLOCKLEN:
	case MMC_CMD_SPI_CRC_ON_OFF:
		resp = &resp8;
		resp_size = sizeof(resp8);
		resp_match = true;
		resp_match_value = 0x0;
		break;
	case MMC_CMD_STOP_TRANSMISSION:
	case MMC_CMD_ERASE:
		resp = &resp8;
		resp_size = sizeof(resp8);
		r1b = true;
		break;
	case MMC_CMD_SEND_CSD:
	case MMC_CMD_SEND_CID:
	case MMC_CMD_READ_SINGLE_BLOCK:
	case MMC_CMD_READ_MULTIPLE_BLOCK:
	case MMC_CMD_WRITE_SINGLE_BLOCK:
	case MMC_CMD_WRITE_MULTIPLE_BLOCK:
	case MMC_CMD_APP_CMD:
	case SD_CMD_ERASE_WR_BLK_START:
	case SD_CMD_ERASE_WR_BLK_END:
		resp = &resp8;
		resp_size = sizeof(resp8);
		break;
	default:
		resp = &resp8;
		resp_size = sizeof(resp8);
		resp_match = true;
		resp_match_value = R1_SPI_IDLE;
		break;
	};

	ret = mmc_spi_sendcmd(dev, cmd->cmdidx, cmd->cmdarg, cmd->resp_type,
			      resp, resp_size, resp_match, resp_match_value, r1b);
	if (ret)
		goto done;

	switch (cmd->cmdidx) {
	case SD_CMD_APP_SEND_OP_COND:
	case MMC_CMD_SEND_OP_COND:
		cmd->response[0] = (resp8 & R1_SPI_IDLE) ? 0 : OCR_BUSY;
		break;
	case SD_CMD_SEND_IF_COND:
	case MMC_CMD_SPI_READ_OCR:
		cmd->response[0] = resp40[4];
		cmd->response[0] |= (uint)resp40[3] << 8;
		cmd->response[0] |= (uint)resp40[2] << 16;
		cmd->response[0] |= (uint)resp40[1] << 24;
		break;
	case MMC_CMD_SEND_STATUS:
		if (resp16[0] || resp16[1])
			cmd->response[0] = MMC_STATUS_ERROR;
		else
			cmd->response[0] = MMC_STATUS_RDY_FOR_DATA;
		break;
	case MMC_CMD_SEND_CID:
	case MMC_CMD_SEND_CSD:
		ret = mmc_spi_readdata(dev, cmd->response, 1, 16);
		if (ret)
			return ret;
		for (i = 0; i < 4; i++)
			cmd->response[i] =
				cpu_to_be32(cmd->response[i]);
		break;
	default:
		cmd->response[0] = resp8;
		break;
	}

	debug("%s: cmd%d resp0=0x%x resp1=0x%x resp2=0x%x resp3=0x%x\n",
	      __func__, cmd->cmdidx, cmd->response[0], cmd->response[1],
	      cmd->response[2], cmd->response[3]);

	if (data) {
		debug("%s: data flags=0x%x blocks=%d block_size=%d\n",
		      __func__, data->flags, data->blocks, data->blocksize);
		multi = (cmd->cmdidx == MMC_CMD_WRITE_MULTIPLE_BLOCK);
		if (data->flags == MMC_DATA_READ)
			ret = mmc_spi_readdata(dev, data->dest,
					       data->blocks, data->blocksize);
		else if  (data->flags == MMC_DATA_WRITE)
			ret = mmc_spi_writedata(dev, data->src,
						data->blocks, data->blocksize,
						multi);
	}

done:
	dm_spi_xfer(dev, 0, NULL, NULL, SPI_XFER_END);

	return ret;
}

// ------------------------------
extra_module_t mmc_module;

uintptr_t handler(module_arg_t arg)
{
	printd("[handler] cmd = %x\n", arg.cmd);

	if (arg.cmd == 0) {
		printd("[handler] test\n");
	}

	return 0;
}

uintptr_t interrupt_handler(module_arg_t arg)
{
	printd("[interrupt_handler] cmd = %x\n", arg.cmd);
	return 0;
}

static int emod_setup(volatile extra_module_t *emod)
{
	if (emod->id != mmc_module.id
	 || emod->is_sharable != mmc_module.is_sharable) {
		return -1;
	}

	emod->handler = mmc_module.handler;
	emod->interrupt_handler = mmc_module.interrupt_handler;

    printd("[emod_setup] emod ptr at 0x%lx\n", (uintptr_t)emod);
	printd("[emod_setup] handler @ 0x%p\n", handler);
	printd("[emod_setup] interrupt_handler @ 0x%p\n", interrupt_handler);

    return 0;
}


// ------------------------------

__attribute__((section(".text.init")))
uintptr_t mmc_init(volatile extra_module_t *emod)
{
	emod_setup(emod);
	reg_map_setup(emod);

	return 0;
}

extra_module_t mmc_module = {
	.id = MOD_NONSHARE_MMC,
	.is_sharable = 0,
	.handler = handler,
	.interrupt_handler = interrupt_handler
};