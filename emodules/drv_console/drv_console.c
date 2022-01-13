#include "drv_console.h"

/* clang-format off */

#define UART_REG_TXFIFO		0 // 0x00 txdata
#define UART_REG_RXFIFO		1 // 0x04 rxdata
#define UART_REG_TXCTRL		2 // 0x08 txctrl
#define UART_REG_RXCTRL		3 // 0x0C rxctrl
#define UART_REG_IE		4 // 0x10 ie (interrupt enable)
#define UART_REG_IP		5 // 0x14 ip (interrupt pending)
#define UART_REG_DIV		6 // 0x18 div (baud rate divisor)

#define UART_TXFIFO_FULL	0x80000000
#define UART_RXFIFO_EMPTY	0x80000000
#define UART_RXFIFO_DATA	0x000000ff
#define UART_TXCTRL_TXEN	0x00010001
#define UART_RXCTRL_RXEN	0x00000001
#define UART_IE             0x00000002

/* clang-format on */

// We cannot use the word DEFAULT for a parameter that cannot be overridden due to -Werror
#ifndef UART_DEFAULT_BAUD
#define UART_DEFAULT_BAUD 115200
#endif
// drv_ctrl_t ctrl = {
// 	.reg_addr = CONSOLE_REG_ADDR,
// 	.reg_size = CONSOLE_REG_SIZE,
// };

static volatile void* uart_base;
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

static int _sifive_uart_getc(void)
{
    u32 ret = get_reg(UART_REG_RXFIFO);
    if (ret & UART_RXFIFO_EMPTY)
            return -1;
    return ret & UART_RXFIFO_DATA;
}


int sifive_uart_getc(void)
{
	int c;
	while ((c = _sifive_uart_getc()) == -1) ;
	return c;
}

int sifive_uart_init(void* base, u32 in_freq, u32 baudrate)
{
    uart_base = base;
    uart_in_freq = in_freq;
    uart_baudrate = baudrate;

    /* Configure baudrate */
    if (in_freq)
        set_reg(UART_REG_DIV, uart_min_clk_divisor(in_freq, baudrate));
    /* Disable interrupts */
    set_reg(UART_REG_IE, UART_IE);
    /* Enable TX */
    set_reg(UART_REG_TXCTRL, UART_TXCTRL_TXEN);
    /* Enable Rx */
    set_reg(UART_REG_RXCTRL, UART_RXCTRL_RXEN);

    return 0;
}

uintptr_t handler(module_arg_t arg)
{
    char ch;
    switch (arg.cmd)
    {
    case MOD_NONSHARE_UART_CMD_PUTC:
        sifive_uart_putc((char)arg.arg[0]);
        return 0;
    case MOD_NONSHARE_UART_CMD_GETC:
        ch = sifive_uart_getc();
        return (uintptr_t)ch;
    default:
        break;
    }

    return 0;
}

uintptr_t interrupt_handler(module_arg_t arg)
{
    if (arg.cmd == 0) {
        return 0;
    }

    return 0;
}

extra_module_t uart_module = {
    .id = 1,
    .is_sharable = 0,
    .handler = handler,
    .interrupt_handler = interrupt_handler
};

__attribute__((section(".text.init")))
uintptr_t uart_module_init(volatile extra_module_t *emod)
{
	// emod->handler = handler;
	// emod->interrupt_handler = interrupt_handler;

    // printd("[dummy_init] emod ptr at 0x%lx\n", (uintptr_t)emod);
	// printd("[dummy_init] handler @ 0x%p\n", handler);
	// printd("[dummy_init] interrupt_handler @ 0x%p\n", interrupt_handler);

    return 0;
}