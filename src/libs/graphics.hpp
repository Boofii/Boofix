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

    static Framebuffer fb;

public:
    static void init(void* mb);
    static void draw(int x, int y, int color);
};