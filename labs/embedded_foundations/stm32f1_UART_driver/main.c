#include "uart.h"
#include <stdint.h>

static void delay(int d);

int main(void){
    uart_init(9600);

    while (1){
        // test calls 
        uart_putc('A');
        delay(1000);
        uart_puts("whats up people of the world!\r\n");
        delay(1000);
    }

}

static void delay(int d){
    while (d > 0)
        d--;
}
