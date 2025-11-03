#include <graphics.hpp>

extern "C" void kmain(void* mb) {
    Graphics::init(mb);
    Graphics::draw(1024/2, 768/2, 0xFFFFFF);

    while (true);
}