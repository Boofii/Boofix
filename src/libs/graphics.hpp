#pragma once
#include <stdint.h>

class Graphics {
private:
    struct Framebuffer {
        uint64_t addr = 0;
        uint32_t pitch = 0;
        uint32_t width = 0;
        uint32_t height = 0;
        uint8_t bpp = 0;
    };

public:
    static Framebuffer fb;

    static void init(void* mb);
    static void draw(int x, int y);
    static void draw_bitmap(int x, int y);
    static void draw_char(char ch, int x, int y);
    static void draw_text(char* text, int x, int y);
    static void set_color(uint32_t color);
};