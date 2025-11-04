#include <console.hpp>
#include <graphics.hpp>

uint32_t Console::cursor_x = 0;
uint32_t Console::cursor_y = 0;

void Console::write(char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        switch (ch) {
            case '\t':
                cursor_x += spacing_x * 4;
                break;
            case '\n':
                cursor_x = 0;
                cursor_y += spacing_y;
                break;
            default:
                if (cursor_x >= Graphics::fb.width) {
                    cursor_x = 0;
                    cursor_y += spacing_y;
                }
                Graphics::draw_char(ch, cursor_x, cursor_y);
                cursor_x += spacing_x;
        }
    }
}

void Console::erase() {
    if (cursor_x == 0) {
        cursor_x = Graphics::fb.width;
        cursor_y -= spacing_y;
        return;
    }
    cursor_x -= spacing_x;
    for (int x = cursor_x; cursor_x >= cursor_x - 32; x--) {
        for (int y = cursor_y; y >= (cursor_y + spacing_y); y++) {
            uint32_t* pixel = (uint32_t*) (Graphics::fb.addr + y * Graphics::fb.pitch + x * Graphics::fb.bpp / 8);
            *pixel = 0x000000;
        }
    }
}