#include <graphics.hpp>

extern "C" void kmain(void* mb) {
    Graphics::init(mb);
    Graphics::set_color(0x33cc33);
    Graphics::draw_char('B', 0, 0);
    Graphics::draw_char('o', 8, 0);
    Graphics::draw_char('o', 16, 0);
    Graphics::draw_char('f', 24, 0);
    Graphics::draw_char('i', 32, 0);
    Graphics::draw_char('x', 40, 0);

    while (true);
}