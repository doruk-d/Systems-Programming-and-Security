#define _DEFAULT_SOURCE

#include <unistd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdalign.h>

#define ALIGNMENT (alignof(max_align_t))

typedef union block_header{
    struct{
        size_t size_total;
        int is_free;
        union block_header *next;
        union block_header *prev;
    };

    max_align_t _align_;
}block_header_t;


static block_header_t *head = NULL;
static block_header_t *tail = NULL;

static inline size_t align(size_t size);
static block_header_t *request_space(size_t total_size);
static block_header_t *find_free_block(size_t total_size);

// aligns the sizes to prevent memory corruption
static inline size_t align(size_t size){
    return (size + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1);
}

// asks for additional memory from OS when needed
static block_header_t *request_space(size_t total_size){
    block_header_t *block = (block_header_t *)sbrk(total_size);

    if (block == (void *)-1)
        return NULL;

    return block;
}

static block_header_t *find_free_block(size_t total_size){
    block_header_t *current = head;
    while (current != NULL){
        if (current->is_free && current->size_total >= total_size)
            return current;

        current = current->next;
    }

    return NULL;
}

void *my_malloc(size_t size){
    if (size > (PTRDIFF_MAX - ALIGNMENT))
        return NULL;

    size_t aligned_size = align(size);
    if (aligned_size > (PTRDIFF_MAX - sizeof(block_header_t)))
        return NULL;


    size_t total_size = aligned_size + sizeof(block_header_t);


    if (head == NULL){
        block_header_t *new_block = request_space(total_size);
        
        if (new_block == NULL)
            return NULL;

        new_block->size_total = total_size;
        new_block->is_free = 0;
        new_block->prev = head;
        new_block->next = tail;
        head = new_block;
        tail = new_block;
        
        return new_block + 1;
    }
    
    block_header_t *free_block = find_free_block(total_size);

    if (free_block != NULL){
        size_t new_size = free_block->size_total - total_size;
        
        if (new_size < (sizeof(block_header_t) + ALIGNMENT)){
            free_block->is_free = 0;
            return free_block + 1;
        }

        // splitting 
        block_header_t *remainder = (block_header_t *)((char *)free_block + total_size);
        remainder->size_total = new_size;
        remainder->is_free = 1;
        remainder->next = free_block->next;
        remainder->prev = free_block;
        
        free_block->size_total = total_size;
        free_block->is_free = 0;
        free_block->next = remainder;
        
        if (remainder->next != NULL)
            remainder->next->prev = remainder;
        else
            tail = remainder;


        return free_block + 1;
    }


    block_header_t *new_free_block = request_space(total_size);

    if (new_free_block == NULL)
        return NULL;

    new_free_block->size_total = total_size;
    new_free_block->is_free = 0;
    new_free_block->prev = tail;
    new_free_block->next = NULL;
    tail->next = new_free_block;

    tail = new_free_block;

    return new_free_block + 1;

}

void my_free(void *ptr){
    if (!ptr)
        return;

    block_header_t *current_ptr = (block_header_t *)ptr - 1;
    
    // double free check
    if (current_ptr->is_free)
        return;

    current_ptr->is_free = 1;
    
    if (current_ptr->prev != NULL && current_ptr->prev->is_free == 1){
        
        current_ptr->prev->size_total += current_ptr->size_total;
        current_ptr->prev->next = current_ptr->next;
        
        if (current_ptr->next != NULL)
            current_ptr->next->prev = current_ptr->prev;

        current_ptr = current_ptr->prev;

    }

    if (current_ptr->next != NULL && current_ptr->next->is_free == 1){

        current_ptr->size_total += current_ptr->next->size_total;

        current_ptr->next = current_ptr->next->next;

        if (current_ptr->next != NULL)
            current_ptr->next->prev = current_ptr;
    }

    if (current_ptr->next == NULL)
        tail = current_ptr;

    if (current_ptr->prev == NULL)
        head = current_ptr;

}

void *my_calloc(size_t nmemb, size_t size){
    if (size != 0 && nmemb > SIZE_MAX / size)
        return NULL;

    unsigned char *mem = my_malloc(nmemb * size);
    if (!mem)
        return NULL;

    for (size_t i = 0; i < nmemb * size; i++)
        *(mem + i)= 0;

    
    return mem;
}

void *my_realloc(void *ptr, size_t size){
    if (ptr == NULL)
        return my_malloc(size);

    if (size == 0){
        my_free(ptr);
        return NULL;
    }

    block_header_t *current = (block_header_t *)ptr - 1;

    size_t size_data = current->size_total - sizeof(block_header_t);

    if (size < size_data)
        size_data = size;

    void *mem = my_malloc(size);
    if (!mem)
        return NULL;

    unsigned char *src = (unsigned char *)ptr;
    unsigned char *dest = (unsigned char *)mem;
    for (size_t i = 0; i < size_data; i++)
        *dest++ = *src++;

    my_free(ptr);

    return mem;
}
