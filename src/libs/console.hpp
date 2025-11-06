#pragma once
#include <stdint.h>

class Console {
private:
    static uint32_t cursor_x;
    static uint32_t cursor_y;

public:
    static void write(char* text);
    static void erase();
};