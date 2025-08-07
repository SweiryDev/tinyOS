#ifndef __DRIVER_SERIAL_H
#define __DRIVER_SERIAL_H

#include <types.h>

// Initialize COM1 serial port and configurations
void init_serial();

// Write a character to the serial port
void serial_write(char a);

// Write a null-terminated string to the serial port
void serial_print(const char *str);

#endif 