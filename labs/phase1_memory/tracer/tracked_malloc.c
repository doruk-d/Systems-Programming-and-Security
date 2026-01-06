#include "tracked_malloc.h"
#include <stdio.h>
#include <stdlib.h>

void *tracked_malloc(size_t size){
    
    fprintf(stderr, "size: %zu\n", size);

    void *arr = malloc(size);
    if (arr == NULL){
        fprintf(stderr, "error: memory could not allocated\n");
        return NULL;
    }
    
    fprintf(stderr, "malloc address: %p\n", arr);

    return arr;
}
void tracked_free(void *ptr){
    fprintf(stderr, "freed address: %p\n", ptr);
    
    free(ptr);
}

