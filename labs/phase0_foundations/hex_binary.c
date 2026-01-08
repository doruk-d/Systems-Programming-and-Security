#include <stdio.h>
#include <stdlib.h>

int hex_char_to_val(char letter){
    int idx_alpha = 0;
    int base = 10;

    if (letter >= 'a' && letter <= 'f') 
        idx_alpha = letter - 'a';     
    else if (letter >= 'A' && letter <= 'F') 
        idx_alpha = letter - 'A';
    else 
        return letter - '0';

    return idx_alpha + base;
}

int check_hex(char *hex){
    if (hex == NULL) return 0;

    int idx = 0;

    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')){
        idx += 2;
        if (hex[idx] == '\0') return 0;
    }
    
    for (; hex[idx] != '\0'; idx++){
        char c = hex[idx];

        if (!((c >= '0' && c <= '9') ||
              (c >= 'A' && c <= 'F') ||
              (c >= 'a' && c <= 'f'))){
            return 0;
        }
    }
    return 1;
}

void convert_hex(char *hex){
    int idx = 0;

    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))
        idx += 2;

    for (;hex[idx] != '\0'; idx++){
        char current = hex[idx];
        int val = hex_char_to_val(current);

        for (int i = 3; i >= 0; i--){
            printf("%d", (val >> i) & 1);
        }
    }
    printf("\n");

}

int main(int argc, char *argv[]){
    if (argc < 2){
        fprintf(stderr, "Error: enter a hex value\n");
        return EXIT_FAILURE;
    }
    
    char *hex = argv[1]; 

    if (!(check_hex(hex))){
        fprintf(stderr, "Error: not a valid hex value\n");
        return EXIT_FAILURE;
    }

    convert_hex(hex);
    return EXIT_SUCCESS;
}
