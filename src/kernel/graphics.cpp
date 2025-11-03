#include <graphics.hpp>

Graphics::Framebuffer Graphics::fb;
uint32_t col = 0xFFFFFF;

extern "C" const unsigned char font[];
extern "C" const unsigned int font_length;

void Graphics::init(void* mb) {
    if (!mb) return;

    uint8_t* ptr = ((uint8_t*) mb) + 8;
    while (true) {
        uint32_t type = *(uint32_t*)ptr;
        uint32_t size = *(uint32_t*)(ptr + 4);

        if (type == 8) {
            fb.addr = *(uint64_t*)(ptr + 8);
            fb.pitch = *(uint32_t*)(ptr + 16);
            fb.width = *(uint32_t*)(ptr + 20);
            fb.height = *(uint32_t*)(ptr + 24);
            fb.bpp = *(uint8_t*)(ptr + 28);
            break;
        }
        if (type == 0) break;

        ptr += (size + 7) & ~7;
    }
}

void Graphics::draw_bitmap(int ix, int iy) {
    int bitmap_width = 512;
    int x = ix;
    int y = iy;

    for (int current = 0; current < font_length; current++) {
        char byte = font[current];
        for (int bit = 7; bit >= 0; bit--) {
            if ((byte >> bit) & 1)
                draw(x, y);
            x++;

            if ((x - ix) >= bitmap_width) {
                x = ix;
                y++;
            }
        }
    }
}

void Graphics::draw_char(char ch, int ix, int iy) {
    int glyph_index = ch - 32;
    int glyph_size = 32;
    int gx = (glyph_index % 16) * glyph_size;
    int gy = (glyph_index / 16) * glyph_size;
    int first_byte = gy * 64 + (gx / 8);
    int end_byte = first_byte + (64 * 31);
    int x = ix;
    int y = iy;

    for (int i = first_byte; i < end_byte; i += 64) {
        for (int current = i; current < (i + 4); current++) {
            char byte = font[current];
            for (int bit = 7; bit >= 0; bit--) {
                if ((byte >> bit) & 1)
                    draw(x, y);
                x++;
            }
        }
        x = ix;
        y++;
    }
}

void Graphics::draw_text(char* text, int ix, int iy) {
    int x = ix;
    int y = iy;
    int spacing = 8;

    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        Graphics::draw_char(ch, x, y);
        x += spacing;
    }
}

void Graphics::draw(int x, int y) {
    if (x < 0 || x >= fb.width || y < 0 || y >= fb.height)
        return;

    uint32_t* pixel = (uint32_t*) (fb.addr + y * fb.pitch + x * fb.bpp / 8);
    *pixel = col;
}

void Graphics::set_color(uint32_t color) {
    col = color;
}