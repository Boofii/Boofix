#include <console.hpp>
#include <graphics.hpp>

uint32_t Console::cursor_x = 0;
uint32_t Console::cursor_y = 0;

uint32_t last_x = 0;
uint32_t last_y = 0;

void Console::write(char* text) {
    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        switch (ch) {
            case '\t':
                last_x = cursor_x;
                cursor_x += Graphics::spacing_x * 4;
                break;
            case '\n':
                last_x = cursor_x;
                last_y = cursor_y;
                cursor_x = 0;
                cursor_y += Graphics::spacing_y;
                break;
            default:
                last_x = cursor_x;
                last_y = cursor_y;
                if (cursor_x >= Graphics::fb.width) {
                    cursor_x = 0;
                    cursor_y += Graphics::spacing_y;
                }
                Graphics::draw_char(ch, cursor_x, cursor_y);
                cursor_x += Graphics::spacing_x;
        }
    }
}

void Console::erase() {
    uint32_t old = Graphics::color;
    Graphics::color = 0x000000;

    uint32_t start_y = Graphics::spacing_y + 4;
    uint8_t* base = (uint8_t*)Graphics::fb.addr;
    uint32_t pitch_bytes = Graphics::fb.pitch;
    uint32_t bpp_bytes   = Graphics::fb.bpp / 8;

    for (uint32_t y = start_y; y < Graphics::fb.height; y++) {
        uint32_t* row = (uint32_t*)(base + y * pitch_bytes);
        for (uint32_t x = 0; x < Graphics::fb.width; x++) {
            row[x] = 0x000000;
        }
    }

    Console::cursor_x = 0;
    Console::cursor_y = Graphics::spacing_y;
    Graphics::color = old;
}