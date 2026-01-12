// built on chip STM32C031C6 
// blinking led

#include <stdint.h>

// hardware registers 'the where'
#define RCC_IOPENR (*(volatile uint32_t *)(0x40021000 + 0x34))
#define GPIOA_MODER (*(volatile uint32_t *)(0x50000000 + 0x00))
#define GPIOA_ODR (*(volatile uint32_t *)(0x50000000 + 0x14))

// hardware settings 'the what'
#define LED_PIN 5

void delay(volatile int count);

int main(){
  // RCC clock enable
  RCC_IOPENR |= (1 << 0);
  // clear the register
  GPIOA_MODER &= ~(3 << (LED_PIN * 2));
  GPIOA_MODER |= (1 << (LED_PIN * 2)); 

  while (1){
    GPIOA_ODR ^= (1 << LED_PIN);
    delay(1000000);
  }

}

void delay(volatile int count){
  while (count > 0){
    count--;
  }

}
