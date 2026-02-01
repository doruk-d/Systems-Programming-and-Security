#include "uart.h"
#include <stdint.h>

#define GPIOA_BASE 0x40010800
#define USART1_BASE 0x40013800
#define SYS_CLOCK 8000000
#define RCC_APB2ENR (*(volatile uint32_t *)(0x40021000 + 0x18))
#define USART1_SR (*(volatile uint32_t *)(USART1_BASE + 0x00))
#define USART1_DR (*(volatile uint32_t *)(USART1_BASE + 0x04))
#define USART1_BRR (*(volatile uint32_t *)(USART1_BASE + 0x08))
#define USART1_CR1 (*(volatile uint32_t *)(USART1_BASE + 0x0C))
#define PIN 9

uart_err_t uart_init(uint32_t baud){
    // step 1 power on by turning clocks on
    RCC_APB2ENR |= (1 << 2) | (1 << 14);

    // step 2 set the modes and cnfs
    volatile uint32_t *reg = (volatile uint32_t *)(GPIOA_BASE + (PIN / 8) * 4);

    *reg &= ~(0xF << ((PIN % 8) * 4));
    *reg |= (0x9 << ((PIN % 8) * 4));

    // step 3 set usart on
    USART1_CR1 |= (1 << 13);

    // step 4 set baud rate f_ck 8mhz baud rate 9600
    uint32_t div = (SYS_CLOCK + (16 * baud) / 2) / (16 * baud);
    if (div > 0xFFFF || div < 1)
        return UART_ERR_INVALID_BAUD;

    USART1_BRR = div;

    // step 5 set transmitter bit on
    USART1_CR1 |= (1 << 3);

    return UART_OK;
}

void uart_putc(char c){
    // check for TXE status if current value shifted transmit the next
    while (!((USART1_SR >> 7) & 1));

    USART1_DR = c;
}

void uart_puts(char *s){
    while (*s)
        uart_putc(*s++);
}
