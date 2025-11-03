#include <graphics.hpp>

Graphics::Framebuffer Graphics::fb;

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

void Graphics::draw(int x, int y, int color) {
    if (x < 0 || x >= fb.width || y < 0 || y >= fb.height)
        return;

    uint32_t* pixel = (uint32_t*) (fb.addr + y * fb.pitch + x * fb.bpp / 8);
    *pixel = color;
}