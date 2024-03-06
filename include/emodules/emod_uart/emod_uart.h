#pragma once

#include <emodules/emodule_id.h>
#include <emodules/emodule_desc.h>
#include <types.h>

typedef struct {
    int (*getc)(void);
    void (*putc)(char);
} emod_uart_api_t;

typedef struct {
    emod_desc_t emod_uart_desc;
    emod_uart_api_t emod_uart_api;
} emod_uart_t;