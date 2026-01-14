#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include "my_malloc.h"

#define BAR 100

void loading_bar(int filled_bar){
    printf("\r");
    printf("[");

    for (int i = 0; i < BAR; i++){
        if (i < filled_bar)
            printf("#");
        else
            printf(" ");

    }
    printf("]");
    printf("%d%%", filled_bar);
    fflush(stdout);
    
}

int main(){
    int count = 100000000;
    void *arr[100] = {0};

    int filled_bar = 0;

    srand(time(NULL));

    while (count > 0){
        // we will use a medium size of a requested size which is 512 bytes and we have +1 for size = 0 case
        size_t size = (rand() % 512) + 1;
        int index = rand() % 100;

        if (arr[index] == NULL){
            arr[index] = my_malloc(size);
        }
        else{
            my_free(arr[index]);
            arr[index] = NULL;
        }

        count--;

        if (count % 1000000 == 0){
            filled_bar++;
            loading_bar(filled_bar);
        }
    }
    printf("\n");

    return 0;
}
