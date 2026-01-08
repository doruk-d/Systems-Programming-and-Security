#### hex_binary.c - Phase 0: Binary Logic Foundations  
---
**Purpose**:  

A command-line tool that converts hexadecimal values to binary representation.  

---
**Usage**  

**Compilation**  

gcc -Wall -Wextra -Wpedantic -Werror -std=c11 hex_binary.c -o hex_binary  

**Running**  
* ./hex_binary 0xDEADBEEF  
* Output:  11011110101011011011111011101111    
  
* ./hex_binary FF (works with or without 0x prefix)  
* Output:  11111111  


**Error Cases**

* ./hex_binary  
* Output: Error: enter a hex value  
* Exit Code: 1 (EXIT_FAILURE)    
  
  
* ./hex_binary 0xZZZZ  
* Output: Error: not a valid hex value  
* Exit Code: 1 (EXIT_FAILURE)  

---
**Engineering Decisions**

**Header Minimalism**

Initial includes:  
....  
#include <string.h>  
....  

I have tried to avoid <string.h> entirely. Functions it includes require iterating over the string. Since I already have to iterate the string to do the conversion, I just can handle the functionality manually during the process. Such as instead of 'strlen', 'hex[idx] = '\0''.  

....  
status = ....  
goto cleanup;  

....  
cleanup:  
    return status  
...  

Initial plan of mine was implement a solution with a single-exit pattern to keep my code clean, however the simplicity of the code and not needing a cleanup pattern in the absence of allocation of memory made me update my approach as simple return values for each error.  

**Error Handling**

I used EXIT_FAILURE (1) and EXIT_SUCCESS (0) macros from <stdlib.h> instead of magic numbers.  
**stdout**: The binary string  
**stderr**: Error messages  

This organization of output allows the tool to be used in pipelines where data is captured but errors are still visible on the console.  

### Implementation Details

**Conversion:** To be more efficient, I used bitwise operations '(value >> i) & 1' to extract bits rather than a lookup table.  

---
**What I Learned**

**Key Insights**

**Stream Discipline:** I learned the split of 'stderr' and 'stdout' is not only a style choice but a critical one for shell scripting. Such as keeping the piped output clean from error messages.  

**Bitwise Logic:** Extracting bits using '>>' '& 1' instead of 16 if statements for each hex character.  

**Return Codes:** Returning the right values allows the shell to know the program failed or succeed. Checked via 'echo $?'.  

