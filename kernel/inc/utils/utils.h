#ifndef __UTILS
#define __UTILS

#include <types.h>

// Get string length
uint64_t str_len(const char* str);

// Reverse string
void reverse(char* str);

// Integer to ASCII
char* itoa(int num, int base);

// Compare strings
int strcmp(const char *s1, const char *s2);

// Set memory
void *memset(void *s, int c, uint64_t n);

#endif