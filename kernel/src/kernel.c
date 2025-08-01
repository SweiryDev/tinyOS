#include <cpu/ports.h>
#include <driver/vga.h>

const char *str = "Hi!";

int main(){
    cleartext_color(COLOR_BLK, COLOR_GRN);
    hide_cursor();

    // VGA Test
    putchar('\n');
    putchar('0');
    putstr(str);
    putstr("\n\nNewLine!");
    scroll_line();

    uint8_t max = 0;
    max = ~max;
    
    // Stress test:
    while(1)
    for(uint8_t i=0; i<max-1; i++){
        putchar(i);
    }

    return 0;
}
