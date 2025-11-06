#include <graphics.hpp>
#include <console.hpp>
#include <idt.hpp>

extern "C" void kmain(void* mb) {
    Graphics::init(mb);
    IDT::init();

    Graphics::color = 0xFFFFFF;
    Graphics::draw_text("|Boofix|", Graphics::fb.width / 2 - 54, 0);
    Console::write("\n");

    while (true);
}