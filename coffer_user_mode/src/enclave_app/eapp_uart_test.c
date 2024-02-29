#include <stdint.h>
#include <types.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <util/register.h>
#include <enclave/enclave_ops.h>

#define UART_REG_ADDR 0x10011000
#define UART_REG_SIZE 0x400

static inline void __raw_writeb(u8 val, volatile void *addr)
{
	asm volatile("sb %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writew(u16 val, volatile void *addr)
{
	asm volatile("sh %0, 0(%1)" : : "r"(val), "r"(addr));
}

static inline void __raw_writel(u32 val, volatile void *addr)
{
	asm volatile("sw %0, 0(%1)" : : "r"(val), "r"(addr));
}

#if __riscv_xlen != 32
static inline void __raw_writeq(u64 val, volatile void *addr)
{
	asm volatile("sd %0, 0(%1)" : : "r"(val), "r"(addr));
}
#endif

static inline u8 __raw_readb(const volatile void *addr)
{
	u8 val;

	asm volatile("lb %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline u16 __raw_readw(const volatile void *addr)
{
	u16 val;

	asm volatile("lh %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

static inline u32 __raw_readl(const volatile void *addr)
{
	u32 val;

	asm volatile("lw %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}

#if __riscv_xlen != 32
static inline u64 __raw_readq(const volatile void *addr)
{
	u64 val;

	asm volatile("ld %0, 0(%1)" : "=r"(val) : "r"(addr));
	return val;
}
#endif

/* FIXME: These are now the same as asm-generic */

#define __io_rbr()		do {} while (0)
#define __io_rar()		do {} while (0)
#define __io_rbw()		do {} while (0)
#define __io_raw()		do {} while (0)

#define readb_relaxed(c)	({ u8  __v; __io_rbr(); __v = __raw_readb(c); __io_rar(); __v; })
#define readw_relaxed(c)	({ u16 __v; __io_rbr(); __v = __raw_readw(c); __io_rar(); __v; })
#define readl_relaxed(c)	({ u32 __v; __io_rbr(); __v = __raw_readl(c); __io_rar(); __v; })

#define writeb_relaxed(v,c)	({ __io_rbw(); __raw_writeb((v),(c)); __io_raw(); })
#define writew_relaxed(v,c)	({ __io_rbw(); __raw_writew((v),(c)); __io_raw(); })
#define writel_relaxed(v,c)	({ __io_rbw(); __raw_writel((v),(c)); __io_raw(); })

#if __riscv_xlen != 32
#define readq_relaxed(c)	({ u64 __v; __io_rbr(); __v = __raw_readq(c); __io_rar(); __v; })
#define writeq_relaxed(v,c)	({ __io_rbw(); __raw_writeq((v),(c)); __io_raw(); })
#endif

#define __io_br()	do {} while (0)
#define __io_ar()	__asm__ __volatile__ ("fence i,r" : : : "memory");
#define __io_bw()	__asm__ __volatile__ ("fence w,o" : : : "memory");
#define __io_aw()	do {} while (0)

#define readb(c)	({ u8  __v; __io_br(); __v = __raw_readb(c); __io_ar(); __v; })
#define readw(c)	({ u16 __v; __io_br(); __v = __raw_readw(c); __io_ar(); __v; })
#define readl(c)	({ u32 __v; __io_br(); __v = __raw_readl(c); __io_ar(); __v; })

#define writeb(v,c)	({ __io_bw(); __raw_writeb((v),(c)); __io_aw(); })
#define writew(v,c)	({ __io_bw(); __raw_writew((v),(c)); __io_aw(); })
#define writel(v,c)	({ __io_bw(); __raw_writel((v),(c)); __io_aw(); })

#if __riscv_xlen != 32
#define readq(c)	({ u64 __v; __io_br(); __v = __raw_readq(c); __io_ar(); __v; })
#define writeq(v,c)	({ __io_bw(); __raw_writeq((v),(c)); __io_aw(); })
#endif

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

// We cannot use the word DEFAULT for a parameter that cannot be overridden due to -Werror
#ifndef UART_DEFAULT_BAUD
#define UART_DEFAULT_BAUD 115200
#endif

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

int _sifive_uart_getc(void)
{
    u32 ret = get_reg(UART_REG_RXFIFO);
    if (ret & UART_RXFIFO_EMPTY)
        return -1;
    return ret & UART_RXFIFO_DATA;
}

#define SIFIVE_SERIAL_RXDATA_EMPTY_SHIFT 31
#define SIFIVE_SERIAL_RXDATA_EMPTY_MASK (1 << SIFIVE_SERIAL_RXDATA_EMPTY_SHIFT)
#define SIFIVE_SERIAL_RXDATA_DATA_SHIFT 0
#define SIFIVE_SERIAL_RXDATA_DATA_MASK (0xff << SIFIVE_SERIAL_RXDATA_DATA_SHIFT)

u8 __ssp_receive_char(bool* is_empty)
{
    u32 v;
    u8 ch;

    v = get_reg(UART_REG_RXFIFO);

    // if (!is_empty)
    // 	WARN_ON(1);
    // else
    // 	*is_empty = (v & SIFIVE_SERIAL_RXDATA_EMPTY_MASK) >>
    // 		SIFIVE_SERIAL_RXDATA_EMPTY_SHIFT;

    *is_empty = (v & SIFIVE_SERIAL_RXDATA_EMPTY_MASK) >> SIFIVE_SERIAL_RXDATA_EMPTY_SHIFT;
    //     return -1;
    // }

    ch = (v & SIFIVE_SERIAL_RXDATA_DATA_MASK) >> SIFIVE_SERIAL_RXDATA_DATA_SHIFT;

    return ch;
}

int sifive_uart_getc(void)
{
    int c;
    // bool is_empty = false;
    // while ((c = _sifive_uart_getc()) == -1);
    c = _sifive_uart_getc();
    // while((c=__ssp_receive_char(&is_empty))&&(!is_empty))
    //     ;
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

    // printf("[uart driver] div: 0x%x\n", get_reg(UART_REG_DIV));
    // printf("[uart driver] ip: 0x%x\n", get_reg(UART_REG_IP));

    return 0;
}

typedef unsigned char byte;

void serial_init(void* base, uint32_t baud)
{
    // TODO (mmap)
    //  ioctl(file,TTY);
    // printf("mapped addr: %p\n", base);
    sifive_uart_init(base, 0, baud);
}

size_t serial_write(const byte b)
{
    sifive_uart_putc(b);
    return 0;
}

static u8 small_buf[128];
static int buf_idx = 0;
byte serial_read()
{
    if (buf_idx) {
        u8 ch = small_buf[--buf_idx];
        // printf("[internal buf] bufidx = %d, buffered char = \\x%2X\n",buf_idx, ch);
        return ch;
    }
    return (byte)_sifive_uart_getc();
}

bool serial_available(void)
{
    unsigned long cnt = 20000000UL;
    // unsigned long cnt = 10000000UL;
    int ch;
    while (cnt--) {
        ch = _sifive_uart_getc();
        if (ch != -1) {
            small_buf[buf_idx++] = (u8)ch;
            // printf("[internal buf] bufidx = %d, buffered char = \\x%2X\n",buf_idx, ch);
            return true;
        }
    }
    return false;
    // return true;
}

int main()
{
    // SerialPort serial = SerialPort();
    // int fd = open("/dev/mem", O_RDWR);
    // printf("fd: %d, offest: %lx\n", fd, UART_REG_ADDR);
    // void* base = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, UART_REG_ADDR);
    void* base = (void *)0xadd00000;
    int pid = getpid();
    serial_init(base, 115200);
    const byte reset_command[] = { '\x56', '\x00', '\x26', '\x00' };
	unsigned long cycle1 = read_csr(cycle);
    for (int j = 0; j < 1000000; j++) {
        // if (j % 10 == 0)
            // printf("[%d] Loop #%d\n", pid, j);
        for (int i = 0; i < 4; i++) {
            serial_write(reset_command[i]);
        }
        byte b;
        int cnt = 4;
        while (cnt--) {
            b = serial_read();
            // printf("\\x%02X", b);
        }
    }
	unsigned long cycle2 = read_csr(cycle);

	printf("[%d] cycle1 = %ld, cycle2 = %ld\n", pid, cycle1, cycle2);
	printf("[%d] cycle diff = %ld\n", pid, cycle2 - cycle1);
    /*
    puts("");
    byte ver_command[] = { '\x56', '\x00', '\x11', '\x00' };
    for (int i = 0; i < 4; i++) {
        serial_write(ver_command[i]);
    }

    // cnt = 100;
    while (serial_available()) {
        b = serial_read();
        // if(isalnum(b)||b=='\n'){
        //     putchar(b);
        // }
        // else{
        //     printf("\\x%02X", b);
        // }
        putchar(b);
    }
    puts("");
    puts("-----end------");
    */
    return 0;
}
