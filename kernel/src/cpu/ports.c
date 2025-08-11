#include <cpu/ports.h>

uint8_t byte_in(uint16_t port){

    // "=a" (result) put value of rax in result
    // "d" (port) put value of port in dx

    // asm instructions use the form "command" : "output" : "input"
    // "in" assembly command to read from a port
    uint8_t result;
    __asm__ __volatile__ ("in %%dx, %%al" : "=a" (result) : "d" (port));
    return result;
}

void byte_out(uint16_t port, uint8_t data){
    // "out" command to write to a port
    __asm__ __volatile__ ("out %%al, %%dx" : : "a" (data), "d" (port));
}

void word_out(uint16_t port, uint16_t data){
    // "out" with ax register for 16-bit data
    __asm__ __volatile__ ("out %%ax, %%dx" : : "a" (data), "d" (port));
}