#ifndef __PORTS
#define __PORTS

#include <types.h>

// Read a byte from port
uint8_t byte_in(uint16_t port);

// Write a byte to port
void byte_out(uint16_t port, uint8_t data);

// Write a word to port (16 bits)
void word_out(uint16_t port, uint16_t data);

#endif