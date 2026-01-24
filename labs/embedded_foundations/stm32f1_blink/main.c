#include <stdint.h>

#define STACK_TOP (0x20000000 + 0x5000) // base plus ram size

#define RCC_APB2ENR (*(volatile uint32_t *)(0x40021000 + 0x18))
#define GPIOA_CRL (*(volatile uint32_t *)(0x40010800 + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(0x40010800 + 0x0C))
#define LED 5

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;


void Reset_Handler(void);
void delay(volatile int count);

__attribute__((section(".isr_vector")))

const uint32_t vector_table[] = {
    (uint32_t)STACK_TOP,
    (uint32_t)Reset_Handler
};

void Reset_Handler(){
    uint32_t *src_data = &_sidata;
    uint32_t *dest_sdata = &_sdata;
    uint32_t *dest_edata = &_edata;

    while (dest_sdata < dest_edata)
        *dest_sdata++ = *src_data++;


    uint32_t *p_sbss = &_sbss;
    uint32_t *p_ebss = &_ebss;

    while (p_sbss < p_ebss)
        *p_sbss++ = 0;

    extern void main(void);
    main();
}


void main(){
    RCC_APB2ENR |= (1 << 2);

    GPIOA_CRL &= ~(0xF << (LED * 4));
    GPIOA_CRL |= (1 << (LED * 4));

    while (1){
        GPIOA_ODR ^= (1 << LED);
        delay(1000);
    }
}

void delay(volatile int count){
    while (count > 0)
        count--;
}

