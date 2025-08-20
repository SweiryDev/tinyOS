#include <utils/utils.h>
#include <memory/heap.h>
#include <driver/vga.h>
#include <types.h>

// Create a duplicate of the string and clear spaces
char* duplicate_trim_leading_spaces(const char* src){
    // Get length and reserve memory
    uint64_t len = str_len(src);
    char* dup_str = (char*)kmalloc(len);

    // Copy string (include null terminator)
    memcpy(dup_str, src, len+1);

    if(len == 0) return dup_str;

    uint64_t i=0;
    while(dup_str[i] == ' ')
        i++;

    dup_str = (dup_str + i);

    return dup_str;
}

uint64_t str_len(const char* str){
    uint64_t counter = 0;

    while(*(str + counter) != '\0'){
        counter++;
    }

    return counter;
}

void reverse(char* str){
    uint64_t i = 0;
    uint64_t j = str_len(str) - 1;

    while (i < j)
    {
        char tmp = str[i];
        str[i] = str[j];
        str[j] = tmp;
        j--;
        i++;
    }   
}

char* itoa(uint64_t num, int base){
    int i=0;
    int isNegative = 0;

    // Allocate 64 characters
    char *str = (char*)kmalloc(64);

    if (str == 0) {
        putstr("itoa: kmalloc failed! Out of heap memory.\n");
        while(1); // Halt the system
    }

    if(num == 0){
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if(num < 0 && base == 10){
        isNegative = 1;
        num = -num;
    }

    while (num != 0){
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'A' : rem + '0';
        num = num / base;
    }

    if(isNegative)
        str[i++] = '-';
    
    str[i] = '\0';

    reverse(str);

    return str;
}

int strcmp(const char *s1, const char *s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void *memset(void *s, int c, uint64_t n) {
    unsigned char *p = s;
    while (n--) {
        *p++ = (unsigned char)c;
    }
    return s;
}

void *memcpy(void *dest, const void *src, uint64_t n){
    uint8_t *pdest = (uint8_t*)dest;
    const uint8_t *psrc = (const uint8_t*)src;

    for (uint64_t i=0; i<n; i++)
        pdest[i] = psrc[i];

    return dest;
}