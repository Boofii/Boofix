#include <graphics.hpp>
#include <console.hpp>
#include <idt.hpp>

uint32_t color(int h, int s, int v) {
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

extern "C" void kmain(void* mb) {
    Graphics::init(mb);
    IDT::init();

    Graphics::set_color(0xFFFFFF);
    Graphics::draw_text("--------------\n===>Boofix<===\n--------------", Graphics::fb.width / 2 - 24, 0);
    Console::write("\n\n\n");
    Graphics::set_color(0x33cc33);

    while (true);
}