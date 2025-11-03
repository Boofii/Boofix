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