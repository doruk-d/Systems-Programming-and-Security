MEMORY
{
    FLASH (rx) : ORIGIN = 0x08000000, LENGTH = 512K
    
    RAM (rwx) : ORIGIN = 0x20000000, LENGTH = 96K
}

SECTIONS
{
    _estack = ORIGIN(RAM) + LENGTH(RAM);

    .text :
    {
        KEEP(*(".isr_vector"))
        *(.text*)
        . = ALIGN(4);
    } > FLASH

    .rodata :
    {
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH


    _sidata = LOADADDR(.data);

    .data : 
    {
        _sdata = .;
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > RAM AT > FLASH

    .bss :
    {
        _sbss = .;
        *(.bss*);
        . = ALIGN(4);
        _ebss = .;
    } > RAM
}
