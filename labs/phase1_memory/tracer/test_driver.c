#include "tracked_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

typedef struct{
    int a;
    char b;
}test_t;

int main(int argc, char *argv[]){
    if (argc != 3){
        printf("error: missing arguments\n");
        return -1;
    }


    char *a1 = argv[1];
    char *a2 = argv[2];
    char *end1;
    char *end2;

    errno = 0;
    long arg1 = strtol(a1, &end1, 10);
    long arg2 = strtol(a2, &end2, 10);

    if (errno == ERANGE){
        fprintf(stderr, "number out of range\n");
        return -1;
    }

    if (end1 == a1 || end2 == a2){
        fprintf(stderr, "invalid numbers\n");
        return -1;
    } 
    
    if (*end1 != '\0' || *end2 != '\0'){
        fprintf(stderr, "invalid trailing characters\n");
    }



    int *arr = tracked_malloc(arg1);
    
    test_t *struct_alloc = tracked_malloc(arg2 * sizeof(test_t));

    tracked_free(arr);
    tracked_free(struct_alloc);

    return 0;
}
