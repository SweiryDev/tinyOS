#ifndef __MEMORY_HEAP
#define __MEMORY_HEAP

#include <types.h>

// Memory block header structure
typedef struct __attribute__((packed)) block_header{
    uint64_t size; // Block size (header included) 
    struct block_header *next; // Pointer to the next free block
} block_header_t;

void init_heap();
void *kmalloc(uint64_t size);
void kfree(void *ptr);


#endif