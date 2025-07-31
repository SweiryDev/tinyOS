#include <types.h>
#include <driver/vga.h>
#include <cpu/ports.h>

// FIX VGA.C dont use global pointers!!!!!!!!!!!!!!

void main(){
    cleartext_color(COLOR_BLK, COLOR_GRN);
    unsigned char* i = 0;
    i[0xB8001] = ' ';

    // putstr("Hello World!");
    
    while(1);
}
