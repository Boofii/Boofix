#pragma once
#include <stdint.h>

class Console {
private:
    static uint32_t cursor_x;
    static uint32_t cursor_y;
    const static uint32_t spacing_x = 8;
    const static uint32_t spacing_y = 16;

public:
    static void write(char* text);
};