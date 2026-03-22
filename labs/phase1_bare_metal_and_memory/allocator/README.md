## Custom Allocator

### Overview

- This project aims to build intuition for low-level memory management and systems programming in C.

### Project Structure
```   
    allocator/
    ├── linker.ld
    ├── makefile
    ├── my_malloc.c
    ├── my_malloc.h
    ├── my_malloc_internal.h
    ├── startup.c
    ├── test_driver.c
    ├── README.md
    └── build/
```

**Note**: Uart driver sourced from
`../../phase3_peripheral_control_and_dma/stm32f401_UART_driver/`



### Development Environment

- The allocator was initially prototyped on x86-64 with the purpose of easier debugging and iteration, before being ported to ARM to target microcontroller environments.
- **Target**: STM32F401RE ARM Cortex-M4

### Allocator Design

- **Strategy**: The allocator implements first-fit allocation with block coalescing through a doubly linked list to reduce fragmentation. 
    - **Trade-off**: Doubly linked list implementation chosen over singly linked list, for the easier coalescing it enables, at the cost of slightly larger metadata allocation.

### Memory Alignment

- As the memory alignment has been a major allocator concern, the allocator enforces architecture-safe alignment rules to prevent misaligned access, data corruption, and undefined behavior via:
    - Inline alignment function that rounds requested sizes to the required alignment boundary,
    - Alignment macro that:
        - Defines the alignment boundary used for rounding allocation sizes
        - Enforces a minimum allocation size to mitigate fragmentation risk
    - Metadata struct encapsulated in a union to guarantee proper header alignment.

### Interrupt Safety

- Allocator operations disable interrupts during critical sections to guarantee atomicity. In interrupt driven environments, this approach provides memory safety by preventing interruption of allocation operations, which could otherwise corrupt the free list and leave the heap in inconsistent state.  

### Heap Lifecycle 

- The allocator uses the function `wipe_heap()` to reinitialize heap metadata and overwrite the memory, preventing information leaks.

### Internal Header

- An internal header exposes the metadata structure to the test driver and source code, without making those part of the public API. 

- This maintains encapsulation while enabling tests to verify the allocator behavior.

### Linker Script Memory Layout

- The linker script places the heap directly after `.bss` section. 
- The stack space is allocated at the top of RAM and grows downward.
- A linker `ASSERT` guarantees that .bss section does not overlap the reserved stack space.

### Stack Safety Instrumentation

- Populated the unallocated stack space with a Stack Canary pattern using the magic number 0xDEADBEEF to detect stack overflow or heap collision.
- Implemented a standard HardFault Handler.

### Build System Decisions

- Employed `vpath` for source discovery alongside `notdir` and `addprefix` to manage object file paths, keeping the Makefile and source directories clean.

### Testing

- Tests include:
    - Validation of stack integrity by walking the stack to the canary boundary and comparing depth against the configured stack size.
    - Verification of allocator behavior under heap exhaustion and full memory reclamation via `wipe_heap()`.
    - Verification of allocator behavior under large continuous allocation and heavy fragmentation.
- Tests run once to prevent results from getting lost in infinite loop output. 

### Bug Discovery

- Uncovered a `size_t` overflow that wrapped the size calculation, undersizing the pointer array and causing out-of-bounds writes during allocation tracking, triggering HardFault.
- Resolved by using a statically sized array with compile-time macros.

### Scope

- As stated, this project is purely educational and is not intended for production use. 
- During the process, developed familiarity with memory layout, fragmentation, atomic sections, and embedded diagnostics as well as experience porting between x86 and ARM architectures.
