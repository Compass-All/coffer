// See LICENSE for license details.

// #include <string.h>
// #include <stdarg.h>
// #include <assert.h>
#include "drv_dummy.h"
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

uintptr_t handler(uint8_t arg)
{
	printd("[handler] arg = %x\n", arg);
	return 0;
}

uintptr_t interrupt_handler(uint8_t arg)
{
	printd("[interrupt_handler] arg = %x\n", arg);
	return 0;
}

uintptr_t dummy_init(volatile extra_module_t *emod) __attribute__((section(".text.init")));
uintptr_t dummy_init(volatile extra_module_t *emod)
{
	emod->handler = handler;
	emod->interrupt_handler = interrupt_handler;

    printd("[dummy_init] emod ptr at 0x%lx\n", (uintptr_t)emod);
	printd("[dummy_init] handler @ 0x%p\n", handler);
	printd("[dummy_init] interrupt_handler @ 0x%p\n", interrupt_handler);

    return 0;
}