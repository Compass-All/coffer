#include <stdint.h>
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

// See LICENSE for license details.

// #include <string.h>
// #include <stdarg.h>
// #include <assert.h>
#include "drv_console.h"

volatile uint8_t *uart16550;
// some devices require a shifted register index
// (e.g. 32 bit registers instead of 8 bit registers)

#define UART_REG_QUEUE 0 // rx/tx fifo data
#define UART_REG_DLL 0	 // divisor latch (LSB)
#define UART_REG_IER 1	 // interrupt enable register
#define UART_REG_DLM 1	 // divisor latch (MSB)
#define UART_REG_FCR 2	 // fifo control register
#define UART_REG_LCR 3	 // line control register
#define UART_REG_MCR 4	 // modem control register
#define UART_REG_LSR 5	 // line status register
#define UART_REG_MSR 6	 // modem status register
#define UART_REG_SCR 7	 // scratch register
#define UART_REG_STATUS_RX 0x01
#define UART_REG_STATUS_TX 0x20

/* clang-format off */

#define UART_REG_TXFIFO		0
#define UART_REG_RXFIFO		1
#define UART_REG_TXCTRL		2
#define UART_REG_RXCTRL		3
#define UART_REG_IE		4
#define UART_REG_IP		5
#define UART_REG_DIV		6

#define UART_TXFIFO_FULL	0x80000000
#define UART_RXFIFO_EMPTY	0x80000000
#define UART_RXFIFO_DATA	0x000000ff
#define UART_TXCTRL_TXEN	0x1
#define UART_RXCTRL_RXEN	0x1

/* clang-format on */

// We cannot use the word DEFAULT for a parameter that cannot be overridden due to -Werror
#ifndef UART_DEFAULT_BAUD
#define UART_DEFAULT_BAUD 115200
#endif

static volatile void *uart_base;
static u32 uart_in_freq;
static u32 uart_baudrate;

/**
 * Find minimum divisor divides in_freq to max_target_hz;
 * Based on uart driver n SiFive FSBL.
 *
 * f_baud = f_in / (div + 1) => div = (f_in / f_baud) - 1
 * The nearest integer solution requires rounding up as to not exceed max_target_hz.
 * div  = ceil(f_in / f_baud) - 1
 *	= floor((f_in - 1 + f_baud) / f_baud) - 1
 * This should not overflow as long as (f_in - 1 + f_baud) does not exceed
 * 2^32 - 1, which is unlikely since we represent frequencies in kHz.
 */
static inline unsigned int uart_min_clk_divisor(uint64_t in_freq,
						uint64_t max_target_hz)
{
	uint64_t quotient = (in_freq + max_target_hz - 1) / (max_target_hz);
	/* Avoid underflow */
	if (quotient == 0) {
		return 0;
	} else {
		return quotient - 1;
	}
}

static u32 get_reg(u32 num)
{
	return readl(uart_base + (num * 0x4));
}

static void set_reg(u32 num, u32 val)
{
	writel(val, uart_base + (num * 0x4));
}

void sifive_uart_putc(char ch)
{
	while (get_reg(UART_REG_TXFIFO) & UART_TXFIFO_FULL)
		;

	set_reg(UART_REG_TXFIFO, ch);
}

int sifive_uart_getc(void)
{
	u32 ret = get_reg(UART_REG_RXFIFO);
	if (!(ret & UART_RXFIFO_EMPTY))
		return ret & UART_RXFIFO_DATA;
	return -1;
}

int sifive_uart_init(unsigned long base, u32 in_freq, u32 baudrate)
{
	uart_base     = (volatile void *)base;
	uart_in_freq  = in_freq;
	uart_baudrate = baudrate;

	/* Configure baudrate */
	if (in_freq)
		set_reg(UART_REG_DIV, uart_min_clk_divisor(in_freq, baudrate));
	/* Disable interrupts */
	set_reg(UART_REG_IE, 0);
	/* Enable TX */
	set_reg(UART_REG_TXCTRL, UART_TXCTRL_TXEN);
	/* Enable Rx */
	set_reg(UART_REG_RXCTRL, UART_RXCTRL_RXEN);

	return 0;
}

uintptr_t cmd_handler(uintptr_t cmd, uintptr_t arg0, uintptr_t arg1,
				uintptr_t arg2)
{
	switch (cmd) {
	case CONSOLE_CMD_INIT:
		return sifive_uart_init(arg0, 0, UART_DEFAULT_BAUD);
	case CONSOLE_CMD_PUT:
		sifive_uart_putc((char)arg0);
		return 0;
	case CONSOLE_CMD_GET:
		return sifive_uart_getc();
	case CONSOLE_CMD_DESTORY:
		return 0;
		// return uart16550_destroy();
	default:
		return -1;
	}
}

uintptr_t handler(module_arg_t arg)
{
	switch (arg.cmd) {
	case CONSOLE_CMD_PUT:
		sifive_uart_putc((char)arg.arg[0]);
		return 0;
	default:
		return -1;
	}
}

uintptr_t uart_init(volatile extra_module_t *emod)
	__attribute__((section(".text.init")));
uintptr_t uart_init(volatile extra_module_t *emod)
{
	printd("[uart_init] heeeeellloooooo\n");
	// sifive_uart_init(arg0, 0, UART_DEFAULT_BAUD);

	emod->handler = handler;

	return 0;
}