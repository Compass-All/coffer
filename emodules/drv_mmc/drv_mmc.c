// See LICENSE for license details.

// #include <string.h>
// #include <stdarg.h>
// #include <assert.h>
#include "drv_mmc.h"
#include "spi.h"
#include <stdint.h>
#include <driver/driver.h>

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>

#define bool char
#define false 0
#define true 1

int vsnprintf(char *out, size_t n, const char *s, va_list vl)
{
	bool format  = false;
	bool longarg = false;
	size_t pos   = 0;
	for (; *s; s++) {
		if (format) {
			switch (*s) {
			case 'l':
				longarg = true;
				break;
			case 'p':
				longarg = true;
				if (++pos < n)
					out[pos - 1] = '0';
				if (++pos < n)
					out[pos - 1] = 'x';
			case 'x': {
				long num = longarg ? va_arg(vl, long)
						   : va_arg(vl, int);
				for (int i = 2 * (longarg ? sizeof(long)
							  : sizeof(int)) -
					     1;
				     i >= 0; i--) {
					int d = (num >> (4 * i)) & 0xF;
					if (++pos < n)
						out[pos - 1] =
							(d < 10 ? '0' + d
								: 'a' + d - 10);
				}
				longarg = false;
				format	= false;
				break;
			}
			case 'd': {
				long num = longarg ? va_arg(vl, long)
						   : va_arg(vl, int);
				if (num < 0) {
					num = -num;
					if (++pos < n)
						out[pos - 1] = '-';
				}
				long digits = 1;
				for (long nn = num; nn /= 10; digits++)
					;
				for (int i = digits - 1; i >= 0; i--) {
					if (pos + i + 1 < n)
						out[pos + i] = '0' + (num % 10);
					num /= 10;
				}
				pos += digits;
				longarg = false;
				format	= false;
				break;
			}
			case 's': {
				const char *s2 = va_arg(vl, const char *);
				while (*s2) {
					if (++pos < n)
						out[pos - 1] = *s2;
					s2++;
				}
				longarg = false;
				format	= false;
				break;
			}
			case 'c': {
				if (++pos < n)
					out[pos - 1] = (char)va_arg(vl, int);
				longarg = false;
				format	= false;
				break;
			}
			default:
				break;
			}
		} else if (*s == '%')
			format = true;
		else if (++pos < n)
			out[pos - 1] = *s;
	}
	if (pos < n)
		out[pos] = 0;
	else if (n)
		out[n - 1] = 0;
	return pos;
}

int snprintf(char *out, size_t n, const char *s, ...)
{
	va_list vl;
	va_start(vl, s);
	int res = vsnprintf(out, n, s, vl);
	va_end(vl);
	return res;
}

void putstring(const char *s)
{
	while (*s) {
		unsigned int arg0		   = (unsigned int)*s++;
		register unsigned int a0 asm("a0") = (unsigned int)(arg0);
		register unsigned int a7 asm("a7") = (unsigned int)(1);
		asm volatile("ecall" : "+r"(a0) : "r"(a7) : "memory");
	}
}
void vprintd(const char *s, va_list vl)
{
	char buf[256];
	vsnprintf(buf, sizeof buf, s, vl);
	putstring(buf);
}

void printd(const char *s, ...)
{
	va_list vl;

	va_start(vl, s);
	vprintd(s, vl);
	va_end(vl);
}

#define debug(fmt, ...) printd("\033[37m[%s] " fmt "\033[0m", __func__, ##__VA_ARGS__)

// ------------------------------

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
 * dependencies:
 * 		- dm_spi_xfer(...)
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
 * dependencies:
 * 		- CONFIG_MMC_SPI_CRC_ON
 * 		- dm_spi_xfer
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
 * dependencies:
 * 		- CONFIG_MMC_SPI_CRC_ON
 * 		- dm_spi_xfer
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

/*
	dependencies:
		- dm_spi_xfer(...);
*/

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

// ------------------------------

extra_module_t dummy_module = {
	.id = MOD_NONSHARE_MMC,
	.is_sharable = 0,
	.handler = handler,
	.interrupt_handler = interrupt_handler
};

__attribute__((section(".text.init")))
uintptr_t dummy_init(volatile extra_module_t *emod)
{
	if (emod->id != dummy_module.id
	 || emod->is_sharable != dummy_module.is_sharable) {
		return -1;
	}

	hello();

	emod->handler = dummy_module.handler;
	emod->interrupt_handler = dummy_module.interrupt_handler;

    printd("[dummy_init] emod ptr at 0x%lx\n", (uintptr_t)emod);
	printd("[dummy_init] handler @ 0x%p\n", handler);
	printd("[dummy_init] interrupt_handler @ 0x%p\n", interrupt_handler);

    return 0;
}