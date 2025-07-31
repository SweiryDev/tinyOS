#ifndef __DRIVER_VGA_TEXT
#define __DRIVER_VGA_TEXT

#include <types.h>

// VGA colors
#define COLOR_BLK 0     // Black
#define COLOR_BLU 1     // Blue
#define COLOR_GRN 2     // Green
#define COLOR_CYN 3     // Cyan
#define COLOR_RED 4     // Red
#define COLOR_PRP 5     // Purple
#define COLOR_BRN 6     // Brown
#define COLOR_GRY 7     // Gray
#define COLOR_DGY 8     // Dark Gray
#define COLOR_LBU 9     // Light Blue
#define COLOR_LGR 10    // Light Green
#define COLOR_LCY 11    // Light Cyan
#define COLOR_LRD 12    // Light Red
#define COLOR_LPP 13    // Light Purple
#define COLOR_YEL 14    // Yellow
#define COLOR_WHT 15    // White

// Default VGA colors
#define DEFAULT_COLOR_FG COLOR_GRN
#define DEFAULT_COLOR_BG COLOR_BLK

// VGA configuration
#define VGA_START 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_EXTENT (VGA_WIDTH * VGA_HEIGHT)

// cursor ports
#define CURSOR_PORT_COMMAND     (uint16_t) 0x3D4
#define CURSOR_PORT_DATA        (uint16_t) 0x3D5


// 2 Byte VGA character styling
typedef struct  __attribute__((packed)) {
    char character;
    char style;
} vga_char;


// Get the char to use as the style char
uint8_t vga_color(const uint8_t fg_color, const uint8_t bg_color);


// Clear the screen with a color
void cleartext();
void cleartext_color(const uint8_t fg_color, const uint8_t bg_color);


// Print a character to the screen
void putchar(const char character);
void putchar_color(const char character, const uint8_t fg_color, const uint8_t bg_color);

// Print a string to the screen
void putstr(const char *string);
void putstr_color(const char *string, const uint8_t fg_color, const uint8_t bg_color);

// Get the cursor position
uint16_t get_cursor_pos();

// Show/Hide Cursor
void show_cursor();
void hide_cursor();

// Set the cursor position
void advance_cursor();
void reverse_cursor();
void set_cursor_pos(uint8_t x, uint8_t y);

// Scroll Line
void scroll_line();

#endif 