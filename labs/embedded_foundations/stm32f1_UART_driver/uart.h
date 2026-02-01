#ifndef UART_H
#define UART_H

#include <stdint.h>

typedef enum{
    UART_OK, UART_ERR_INVALID_BAUD, UART_ERR_TIMEOUT    
}uart_err_t;

uart_err_t uart_init(uint32_t baud);
void uart_putc(char c);
void uart_puts(char *s);

#endif
