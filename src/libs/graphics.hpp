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
    static uint32_t spacing_x;
    static uint32_t spacing_y;
    static uint32_t color;

    static void init(void* mb);
    static void draw(uint32_t x, uint32_t y);
    static void draw_bitmap(uint32_t x, uint32_t y);
    static void draw_char(char ch, uint32_t x, uint32_t y);
    static void draw_text(char* text, uint32_t x, uint32_t y);
    static uint32_t hsv(uint32_t h, uint32_t s, uint32_t v);
};