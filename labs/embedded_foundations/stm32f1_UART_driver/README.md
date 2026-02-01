## STM32F103 Asynchronous Duplex UART Driver 

### Overview

A lab project to experiment and learn bare metal coding on STM32F103 to develop a basic asynchronous UART driver built on USART1. The goal is to provide transmission and recieving functions.

```
Files:
    |
    +-- setup.resc
    +-- linker.ld
    +-- startup.c
    +-- makefile
    +-- uart.c
    +-- uart.h
    +-- main.c
```

### Features

* Basic Asynchronous UART driver using USART1
* Transmission functionality 
* Receiving functionality
* Flush function to ensure data is sent or received completely without any buffer overflow //apply it for the receiving too

### Implementation Details

* **Reference Materials**: Used STM32F103x Reference Manual (RM0008) along the process for guidance.
* **Hardware Understanding**: Implemented by using Direct Register Access (DRA) to ensure full transparency of the Hardware-Software Interface (HSI).
* **Memory Address Calculation**: Used a generic formula to calculate memory addresses for CRL/H registers in the code.
* **Polling-Based Approach**: Utilizes a synchronous polling architecture for TX/RX operations. Acknowledging its limitations as blocking and preventing concurrent execution during I/O wait states.
* **Error Handling**: Incorporated an enum in the header file for common error checks, enhancing driver responsiveness.
* **Security Measure**:Implemented input validation in `uart_puts` to mitigate information leakage caused by NULL pointer dereferencing of the aliased Vector Table.
* **Baud Rate Calculation**: Implemented fixed point integer arithmetic that accurately calculates Baud Rate Generator values without the use of floating point unit.
* **Main function**: Test driver for UART functions. Prints an entry message, takes input from an interface and echoes back received data.

### Core Infrastructure

* These files provide the foundation for the driver, setup and initialization.
```
    |
    +-- linker.ld: Linker script for memory layout
    +-- startup.c: Reset the system using the ResetHandler function and sets up the Vector Table.
    +-- setup.resc: Setup script for Renode
```

### Hardware Configuration
* TX Pin: PA9 (Alternate function push-pull)
* RX Pin: PA10 (Floating input)
* Clock: APB2 (72MHz max, configured 8MHz using HSI)
