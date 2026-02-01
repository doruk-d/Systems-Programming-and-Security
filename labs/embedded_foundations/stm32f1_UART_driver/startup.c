#include <stdint.h>

#define STACK_TOP (0x20000000 + 0x5000)

extern uint32_t _sidata;
extern uint32_t _sdata;
extern uint32_t _edata;
extern uint32_t _sbss;
extern uint32_t _ebss;

void ResetHandler(void);

__attribute__((section(".isr_vector")))

const uint32_t vector_table[]={
    (uint32_t)STACK_TOP,
    (uint32_t)ResetHandler
};

void ResetHandler(void){
}
