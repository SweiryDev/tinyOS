#include <memory/heap.h>
#include <types.h>

// Static array for heap (1MB)
#define KERNEL_HEAP_SIZE (1024 * 1024)
static uint8_t kernel_heap[KERNEL_HEAP_SIZE];

static block_header_t *free_list_head = 0;

void init_heap(){
    uint64_t heap_start = (uint64_t) kernel_heap;

    // First free block is the entire heap space
    free_list_head = (block_header_t*) heap_start;
    free_list_head->size = KERNEL_HEAP_SIZE;
    free_list_head->next = 0;
}

void *kmalloc(uint64_t size){
    uint64_t aligned_size = (size + 15) & ~15;
    uint64_t required_size = aligned_size + sizeof(block_header_t);

    block_header_t *current = free_list_head;
    block_header_t *previous = 0;

    while(current){
        if(current->size >= required_size){
            // Found block of required size
            uint64_t remaining_size = current->size - required_size;

            if(remaining_size > sizeof(block_header_t)){
                // Large enough to split
                block_header_t *new_free_block = (block_header_t*)((uint8_t*)current + required_size);
                new_free_block->size = remaining_size;
                new_free_block->next = current->next;

                if(previous){
                    previous->next = new_free_block;
                }else{
                    free_list_head = new_free_block;
                }

                // Resize allocated block
                current->size = required_size; 
            }else{
                // Block is a perfect fit, remove it from the list
                if(previous){
                    previous->next = current->next;
                }else{
                    free_list_head = current->next;
                }
            }

            // Return a pointer to the data area
            return (void*) ((uint8_t*) current + sizeof(block_header_t));
        }
        previous = current;
        current = current->next;
    }

    // No block to allocate...
    return 0;
}

void kfree(void *ptr){
    if(!ptr){
        return;
    }

    block_header_t *block_to_free = (block_header_t*)((uint8_t*)ptr - sizeof(block_header_t));

    block_header_t *current = free_list_head;
    block_header_t *previous = 0;

    // Find the correct place to insert the block
    while(current != 0 && current < block_to_free){
        previous = current;
        current = current->next;
    }

    if(previous == 0){
        // Insert at the head of the list
        block_to_free->next = free_list_head;
        free_list_head = block_to_free;
    }else{
        // Insert in the middle or at the end
        previous->next = block_to_free;
        block_to_free->next = current;
    }

    // Coalesce with the next block
    if(block_to_free->next != 0 &&
      (uint8_t*)block_to_free + block_to_free->size == (uint8_t*)block_to_free->next){
        block_to_free->size += block_to_free->next->size;
        block_to_free->next = block_to_free->next->next;
    }

    // Coalesce with the previous block
    if(previous != 0 &&
      (uint8_t*)previous + previous->size == (uint8_t*)block_to_free){
        previous->size += block_to_free->size;
        previous->next = block_to_free->next;
      }
}

