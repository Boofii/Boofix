#include <graphics.hpp>
#include <console.hpp>

extern "C" void kmain(void* mb) {
    Graphics::init(mb);
    Graphics::set_color(0x33cc33);
    Console::write("Boofix\n");

    while (true);
}