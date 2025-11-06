#include <graphics.hpp>

Graphics::Framebuffer Graphics::fb;
uint32_t Graphics::color = 0xFFFFFF;
uint32_t Graphics::spacing_x = 14;
uint32_t Graphics::spacing_y = 24;

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

void Graphics::draw_bitmap(uint32_t ix, uint32_t iy) {
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

void Graphics::draw_char(char ch, uint32_t ix, uint32_t iy) {
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

void Graphics::draw_text(char* text, uint32_t ix, uint32_t iy) {
    int x = ix;
    int y = iy;

    for (int i = 0; text[i] != '\0'; i++) {
        char ch = text[i];
        switch (ch) {
            case '\t':
                x += spacing_x * 4;
                break;
            case '\n':
                x = ix;
                y += spacing_y;
                break;
            default:
                draw_char(ch, x, y);
                x += spacing_x;
        }
    }
}

void Graphics::draw(uint32_t x, uint32_t y) {
    if (x < 0 || x >= fb.width || y < 0 || y >= fb.height)
        return;

    uint32_t* pixel = (uint32_t*) (fb.addr + y * fb.pitch + x * fb.bpp / 8);
    *pixel = color;
}

uint32_t Graphics::hsv(uint32_t h, uint32_t s, uint32_t v) {
    int region = h / 60;
    int remainder = (h % 60) * 255 / 60;
    int p = (v * (255 - s)) / 255;
    int q = (v * (255 - (s * remainder) / 255)) / 255;
    int t = (v * (255 - (s * (255 - remainder)) / 255)) / 255;

    int r=0, g=0, b=0;
    if (region == 0) { r = v; g = t; b = p; }
    if (region == 1) { r = q; g = v; b = p; }
    if (region == 2) { r = p; g = v; b = t; }
    if (region == 3) { r = p; g = q; b = v; }
    if (region == 4) { r = t; g = p; b = v; }
    if (region == 5) { r = v; g = p; b = q; }

    uint32_t R = r & 0xFF;
    uint32_t G = g & 0xFF;
    uint32_t B = b & 0xFF;
    uint32_t A = 255;

    return (A << 24) | (R << 16) | (G << 8) | B;
}