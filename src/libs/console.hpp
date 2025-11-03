#pragma once
#include <stdint.h>

class Console {
private:
    const static uint32_t spacing_x = 8;
    const static uint32_t spacing_y = 16;
    static uint32_t cursor_x;
    static uint32_t cursor_y;

public:
    static void write(char* text);
};