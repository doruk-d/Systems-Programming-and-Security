#define _DEFAULT_SOURCE /* since the c11 does not include sbrk by default, need to use this macro to include
                        default Unix extensions */
#include <stdio.h>
#include <unistd.h>
#include <stddef.h>

#define ALLIGN(x) (((x) + 7) & ~7)
#define MIN_ALLOC_SIZE 16 /* because payload parts in the struct usually recycled but not the pointers since 
                          i have 2 its better to choose 16 */

typedef struct block_header{
    size_t size;
    int free;
    struct block_header *next;
    struct block_header *prev;
}block_header_t;


static block_header_t *head= NULL;
static block_header_t *tail= NULL;


static block_header_t *find_free_block(size_t size){
    block_header_t *current = head;

    while (current != NULL){
        if (current->free && current->size >= size){
            return current;
        }
        current = current->next;
    }

    return NULL;    
}

static block_header_t *request_space(size_t size){
    block_header_t *block;
    block = (block_header_t *)sbrk(size + sizeof(block_header_t));
    if (block == (void*)-1)
        return NULL;

    if (head == NULL){
        block->next = head;
        head = block;
    }

    block->size = size;
    block->free = 0;
    block->next = NULL;
    block->prev = tail;

    if (tail != NULL)
        tail->next = block;

    tail = block;

    return block;
}

void *my_malloc(size_t size){
    size = ALLIGN(size);
    if (size < MIN_ALLOC_SIZE) size = 16;

    block_header_t *free_block = find_free_block(size);

    if (free_block){
        if ((free_block->size - size) < (sizeof(block_header_t) + MIN_ALLOC_SIZE)){
            free_block->free = 0;
            return (void *)(free_block + 1);    
        }

        block_header_t *new_block = (block_header_t*)((char*)free_block + sizeof(block_header_t) + size);
        new_block->size = free_block->size - size;
        new_block->free = 1;
        new_block->next = free_block->next;
        new_block->prev = free_block;

        if (new_block->next != NULL)
            new_block->next->prev = new_block;
        
        if (tail == free_block) tail = new_block;

        free_block->size = size;
        free_block->free = 0;
        free_block->next = new_block;
        return (void *)(free_block + 1);
    }


    if (free_block == NULL){
        block_header_t *block = request_space(size);
        if (block == NULL){
            return NULL;
        }
        
        return block + 1;
    }

    return NULL;
}

void my_free(void *ptr){
    if (ptr == NULL)
        return;
    
    block_header_t *header = (block_header_t *)ptr - 1;
    header->free = 1;

    if (header->next != NULL && header->next->free == 1){
        block_header_t *next_block = header->next;
        header->size += next_block->size + sizeof(block_header_t);
        header->next = next_block->next;
        if (header->next != NULL)
            header->next->prev = header;

        if (tail == next_block)
            tail = header;
    }

    if (header->prev != NULL && header->prev->free == 1){
        block_header_t *prev_block = header->prev;
        prev_block->size += header->size + sizeof(block_header_t);
        prev_block->next = header->next; 
        if (prev_block->next != NULL)
            prev_block->next->prev = prev_block;

        if (tail == header)
            tail = prev_block;
    }

}
