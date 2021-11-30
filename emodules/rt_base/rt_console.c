#include "rt_console.h"
#include "base_util/memory.h"
#include "m3/page_table.h"
#include "rt_csr.h"
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CONSOLE_BUF_SIZE 256
#define CONSOLE_USE_ECALL_PUTS

#define FLAG_PAD_RIGHT 1
#define FLAG_PAD_ZERO 2

typedef uint8_t fmt_flag_t;

static char _print_buf[CONSOLE_BUF_SIZE + 1];

#define flush_buffer_if_overflow(buf, pos, width)  \
    do {                                           \
        if ((pos) + (width) >= CONSOLE_BUF_SIZE) { \
            buf[pos] = '\0';                       \
            putstring(buf);                        \
            (pos) = 0;                             \
        }                                          \
    } while (0)

#define pad_fmt(buf, pos, pad_width, flags)                \
    do {                                                   \
        int __tmp = 0;                                     \
        char __fill = ((flags)&FLAG_PAD_ZERO) ? '0' : ' '; \
        while (__tmp < (pad_width)) {                      \
            flush_buffer_if_overflow(buf, pos, 1);         \
            buf[(pos)++] = __fill;                         \
            ++__tmp;                                       \
        }                                                  \
    } while (0)

static void putstring(char* s)
{
#ifdef CONSOLE_USE_ECALL_PUTS
    char* tmp = _print_buf;
    while ((*tmp++ = *s++))
        ;
    uintptr_t pa = rt_get_pa((uintptr_t)_print_buf);
    ecall_puts(pa);
#else
    while (*s) {
        ecall_putchar(*s++);
    }
#endif
}

static void print_s(char* buf, int* i, char* str, int fmt_width, fmt_flag_t flags)
{
    int len = 0, t = 0;
    int cur_len;
    char* ps = str;
    while (*ps) {
        ++len;
        ++ps;
    }

    if (fmt_width > len && flags & FLAG_PAD_RIGHT) {
        pad_fmt(buf, *i, fmt_width - len, flags);
    }

    while (t < len) {
        cur_len = CONSOLE_BUF_SIZE - *i;
        if (len - t < cur_len) {
            cur_len = len - t;
        }
        flush_buffer_if_overflow(buf, *i, cur_len);
        memcpy(&buf[*i], &str[t], cur_len);
        *i += cur_len;
        t += cur_len;
    }

    if (fmt_width > len && !(flags & FLAG_PAD_RIGHT)) {
        pad_fmt(buf, *i, fmt_width - len, flags);
    }
}

static void print_dec(char* buf, int* i, uint64_t num, int fmt_width, fmt_flag_t flags)
{
    int len = 0, t = 0;
    uint64_t tmp = num;

    while (tmp) {
        ++len;
        tmp /= 10;
    }

    if (fmt_width > len && flags & FLAG_PAD_RIGHT) {
        pad_fmt(buf, *i, fmt_width - len, flags);
    }

    if (num == 0) {
        flush_buffer_if_overflow(buf, *i, 1);
        buf[(*i)++] = '0';
    } else {
        flush_buffer_if_overflow(buf, *i, len);
        for (t = *i + len; t > *i;) {
            buf[--t] = '0' + num % 10;
            num /= 10;
        }
        *i += len;
    }

    if (fmt_width > len && !(flags & FLAG_PAD_RIGHT)) {
        pad_fmt(buf, *i, fmt_width - len, flags);
    }
}

static void print_hex(char* buf, int* i, uint64_t num, int fmt_width, fmt_flag_t flags, bool is_upper)
{
    int len = 0, t = 0;
    uint64_t tmp = num;

    while (tmp) {
        ++len;
        tmp >>= 4;
    }

    if (fmt_width > len && flags & FLAG_PAD_RIGHT) {
        pad_fmt(buf, *i, fmt_width - len, flags);
    }

    if (num == 0) {
        flush_buffer_if_overflow(buf, *i, 1);
        buf[(*i)++] = '0';
    } else {
        flush_buffer_if_overflow(buf, *i, len);
        for (t = *i + len; t > *i;) {
            tmp = num & 0xF;
            if (tmp < 10) {
                buf[--t] = '0' + tmp;
            } else {
                buf[--t] = (is_upper ? 'A' : 'a') + tmp - 10;
            }
            num >>= 4;
        }
        *i += len;
    }

    if (fmt_width > len && !(flags & FLAG_PAD_RIGHT)) {
        pad_fmt(buf, *i, fmt_width - len, flags);
    }
}

static void rt_print(const char* fmt, va_list vl)
{
    static char buf[CONSOLE_BUF_SIZE + 1];
    fmt_flag_t fmt_flags = 0;
    int acnt = 0, fmt_width;
    int i = 0;
    uint64_t num;
    char* s;

    for (; *fmt; ++fmt) {
        if (*fmt == '%') {
            ++fmt;
            fmt_width = fmt_flags = 0;
            if (*fmt == '\0') {
                break;
            }
            if (*fmt == '%') {
                goto out;
            }

            // Parse flags
            if (*fmt == '-') {
                ++fmt;
                fmt_flags |= FLAG_PAD_RIGHT;
            }
            while (*fmt == '0') {
                ++fmt;
                fmt_flags |= FLAG_PAD_ZERO;
            }

            // Parse width
            for (; *fmt >= '0' && *fmt <= '9'; ++fmt) {
                fmt_width *= 10;
                fmt_width += *fmt - '0';
            }

            switch (*fmt) {
            case 's':
                s = va_arg(vl, char*);
                acnt += sizeof(char*);
                print_s(buf, &i, s ? s : "(null)", fmt_width, fmt_flags);
                break;

            case 'd':
                num = va_arg(vl, int);
                acnt += sizeof(int);
                if (((int)num) < 0) {
                    flush_buffer_if_overflow(buf, i, 1);
                    buf[i++] = '-';
                    print_dec(buf, &i, -num, fmt_width - 1, fmt_flags);
                } else {
                    print_dec(buf, &i, num, fmt_width, fmt_flags);
                }
                break;

            case 'x':
                num = va_arg(vl, unsigned int);
                acnt += sizeof(unsigned int);
                print_hex(buf, &i, num, fmt_width, fmt_flags, false);
                break;

            case 'X':
                num = va_arg(vl, unsigned int);
                acnt += sizeof(unsigned int);
                print_hex(buf, &i, num, fmt_width, fmt_flags, true);
                break;

            case 'u':
                num = va_arg(vl, unsigned int);
                acnt += sizeof(unsigned int);
                print_dec(buf, &i, num, fmt_width, fmt_flags);
                break;

            case 'p':
                num = va_arg(vl, uintptr_t);
                acnt += sizeof(uintptr_t);
                print_hex(buf, &i, num, fmt_width, fmt_flags, false);
                break;

            case 'P':
                num = va_arg(vl, uintptr_t);
                acnt += sizeof(uintptr_t);
                print_hex(buf, &i, num, fmt_width, fmt_flags, true);
                break;

            case 'l':
                if (*(fmt + 1) == 'l') {
                    while (acnt & (sizeof(uint64_t) - 1)) {
                        va_arg(vl, int);
                        acnt += sizeof(int);
                    }
                    num = va_arg(vl, uint64_t);
                    acnt += sizeof(uint64_t);
                    switch (*(fmt + 2)) {
                    case 'u':
                        fmt += 2;
                        print_dec(buf, &i, num, fmt_width, fmt_flags);
                        break;

                    case 'x':
                        fmt += 2;
                        print_hex(buf, &i, num, fmt_width, fmt_flags, false);
                        break;

                    case 'X':
                        fmt += 2;
                        print_hex(buf, &i, num, fmt_width, fmt_flags, true);
                        break;

                    case 'd':
                        ++fmt;
                    default:
                        ++fmt;
                        if (((int64_t)num) < 0) {
                            flush_buffer_if_overflow(buf, i, 1);
                            buf[i++] = '-';
                            print_dec(buf, &i, -num, fmt_width - 1, fmt_flags);
                        } else {
                            print_dec(buf, &i, num, fmt_width, fmt_flags);
                        }
                        break;
                    }
                } else {
                    num = va_arg(vl, uint32_t);
                    acnt += sizeof(uint32_t);
                    switch (*(fmt + 1)) {
                    case 'u':
                        ++fmt;
                        print_dec(buf, &i, num, fmt_width, fmt_flags);
                        break;

                    case 'x':
                        ++fmt;
                        print_hex(buf, &i, num, fmt_width, fmt_flags, false);
                        break;

                    case 'X':
                        ++fmt;
                        print_hex(buf, &i, num, fmt_width, fmt_flags, true);
                        break;

                    case 'd':
                        ++fmt;
                    default:
                        if (((int32_t)num) < 0) {
                            flush_buffer_if_overflow(buf, i, 1);
                            buf[i++] = '-';
                            print_dec(buf, &i, -num, fmt_width - 1, fmt_flags);
                        } else {
                            print_dec(buf, &i, num, fmt_width, fmt_flags);
                        }
                        break;
                    }
                }
                break;

            case 'c':
                acnt += sizeof(int);
                flush_buffer_if_overflow(buf, i, 1);
                buf[i++] = (char)(va_arg(vl, int));
                break;

            default:
                // Ignore formatting
                break;
            }
        } else {
        out:
            flush_buffer_if_overflow(buf, i, 1);
            buf[i++] = *fmt;
        }
    }
    flush_buffer_if_overflow(buf, i, CONSOLE_BUF_SIZE);
}

void rt_printf(const char* s, ...)
{
    va_list vl;

    va_start(vl, s);
    rt_print(s, vl);
    va_end(vl);
}