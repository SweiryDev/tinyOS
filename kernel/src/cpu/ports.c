
#include <cpu/ports.h>

uint8_t byte_in(uint16_t port){

    // "=a" (result) put value of rax in result
    // "d" (port) put value of port in dx

    // asm instructions use the form "command" : "output" : "input"
    // Use the "in" assembly command to read from a port
    uint8_t result;
    __asm__ volatile ("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}


void byte_out(uint16_t port, uint8_t data){
    // Use the "out" command to write to a port
    __asm__ volatile ("out %%al, %%dx" : : "a" (data), "d" (port));
}