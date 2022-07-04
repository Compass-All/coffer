#include "dependency.h"
#include "serial.h"
#include "tmp_printf.h"
#include <emodules/emod_uart/emod_uart.h>
#include <memory/page_table.h>
#include <util/gnu_attribute.h>

#if defined __UNMATCHED__
#define UART_REG_ADDR 0x10011000UL
#elif defined __QEMU__
#define UART_REG_ADDR 0x101000UL
#endif
#define UART_REG_VA 0xBEE0000000UL
#define UART_REG_SIZE 0x400UL

static emod_desc_t emod_uart_desc = {
    .emod_id = EMODULE_ID_UART,
    .name = "emodule uart",
    .__signature = 0,
};

static emod_uart_api_t emod_uart_api;
static emod_uart_t emod_uart;

// #ifdef EMODULES_DEBUG
#define debug(fmt, ...) printf_(fmt, ##__VA_ARGS__)
// #else
// #define debug(fmt, ...)
// #endif

static emod_uart_t get_emod_uart(void)
{
    return emod_uart;
}

__attribute__((section(".text.init")))
vaddr_t
uart_init(vaddr_t emod_manager_getter)
{
    vaddr_t uart_va = UART_REG_VA;

    emod_uart_api.getc = sifive_uart_getc;
    emod_uart_api.putc = sifive_uart_putc;

    emod_uart = (emod_uart_t) {
        .emod_uart_desc = emod_uart_desc,
        .emod_uart_api = emod_uart_api,
    };

	// record emod_manager
	if (emod_manager_getter == (vaddr_t)0UL) {
		return (vaddr_t)0UL;
	}
	emod_manager_t (*get_emod_manager)(void) = 
		(void *)emod_manager_getter;
	emod_manager = get_emod_manager();
	emod_manager.emod_manager_api.test();

    map_page(uart_va, (paddr_t)UART_REG_ADDR, PTE_V | PTE_R | PTE_W, SV39_LEVEL_PAGE);

    sifive_uart_init((void*)uart_va, 0, 115200);

    return (vaddr_t)get_emod_uart;
}