#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY ((volatile uint16_t*)0xB8000)

void clear(uint8_t color = 0x0E) {
    uint16_t blank = ((uint16_t)color << 8) | ' ';
    for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
        VGA_MEMORY[i] = blank;
    }
}

void write(const char* text, uint8_t color = 0x0E) {
    volatile uint16_t* vga = VGA_MEMORY;
    for (int i = 0; text[i] != '\0'; i++) {
        vga[i] = (0x0E << 8) | text[i];
    }
}

extern "C" void kmain() {
    clear();
    write("Boofix", 0x0E);
    while (true);
}