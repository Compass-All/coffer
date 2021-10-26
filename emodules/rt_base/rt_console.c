#include "rt_console.h"
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define CONSOLE_BUF_SIZE 256

#define FLAG_PAD_RIGHT 1
#define FLAG_PAD_ZERO 2

typedef uint8_t fmt_flag_t;

static void putstring(const char* s)
{
    while (*s) {
        ecall_putchar(*s++);
    }
}

#define flush_buffer_if_overflow(buf, pos, width) \
    do {                                          \
        if (pos + (width) >= CONSOLE_BUF_SIZE) {  \
            buf[pos] = '\0';                      \
            putstring(buf);                       \
            pos = 0;                              \
        }                                         \
    } while (0)

// FIXME wip to replace below function
static void rt_print(const char* fmt, va_list vl)
{
    char buf[CONSOLE_BUF_SIZE];
    fmt_flag_t fmt_flags = 0;
    int acnt, fmt_width;
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
            for (; isdigit(*fmt); ++fmt) {
                fmt_width *= 10;
                fmt_width += *fmt - '0';
            }

            switch (*fmt) {
            case 's':
                s = va_arg(vl, char*);
                acnt += sizeof(char*);
                // print_s(&buf, &i, s, fmt_width, fmt_flags);
                break;
            case 'd':
                num = va_arg(vl, int);
                acnt += sizeof(int);
                // print_dec(&buf, &i, num, fmt_width, fmt_flags, 0);
                break;
            case 'x':
                num = va_arg(vl, unsigned int);
                acnt += sizeof(unsigned int);
                // print_hex(&buf, &i, num, fmt_width, fmt_flags, 0);
                break;
            case 'X':
                num = va_arg(vl, unsigned int);
                acnt += sizeof(unsigned int);
                // print_hex(&buf, &i, num, fmt_width, fmt_flags, 1);
                break;
            case 'u':
                num = va_arg(vl, unsigned int);
                acnt += sizeof(unsigned int);
                // print_dec(&buf, &i, num, fmt_width, fmt_flags, 0);
                break;
                break;
            case 'p':
                num = va_arg(vl, uintptr_t);
                acnt += sizeof(uintptr_t);
                // print_hex(&buf, &i, num, fmt_width, fmt_flags, 0);
                break;
            case 'P':
                num = va_arg(vl, uintptr_t);
                acnt += sizeof(uintptr_t);
                // print_hex(&buf, &i, num, fmt_width, fmt_flags, 1);
                break;
            case 'l':
                if (*(fmt + 1) == 'l') {
                    while (acnt & (sizeof(uint64_t) - 1)) {
                        va_arg(vl, int);
                        acnt += sizeof(int);
                    }
                    num = va_arg(vl, uint64_t);
                    acnt += sizeof(uint64_t);
                    ++fmt;

                } else {
                }
                // ...
                break;
            case 'c':
                acnt += sizeof(int);
                flush_buffer_if_overflow(buf, i, 1);
                buf[i++] = (char)(va_arg(vl, int));
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

static int rt_vsnprintf(char* out, size_t n, const char* s, va_list vl)
{
    bool format = false;
    bool longarg = false;
    size_t pos = 0;
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
                for (int i = 2 * (longarg ? sizeof(long) : sizeof(int)) - 1;
                     i >= 0; i--) {
                    int d = (num >> (4 * i)) & 0xF;
                    if (++pos < n)
                        out[pos - 1] = (d < 10 ? '0' + d
                                               : 'a' + d - 10);
                }
                longarg = false;
                format = false;
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
                format = false;
                break;
            }
            case 's': {
                const char* s2 = va_arg(vl, const char*);
                while (*s2) {
                    if (++pos < n)
                        out[pos - 1] = *s2;
                    s2++;
                }
                longarg = false;
                format = false;
                break;
            }
            case 'c': {
                if (++pos < n)
                    out[pos - 1] = (char)va_arg(vl, int);
                longarg = false;
                format = false;
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

static void rt_vprintf(const char* s, va_list vl)
{
    char buf[256];
    rt_vsnprintf(buf, sizeof(buf), s, vl);
    putstring(buf);
}

void rt_printf(const char* s, ...)
{
    va_list vl;

    va_start(vl, s);
    rt_vprintf(s, vl);
    va_end(vl);
}