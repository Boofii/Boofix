#include <idt.hpp>
#include <console.hpp>

IDT::IDTEntry IDT::idt[256];
IDT::IDTR IDT::idtr;

extern "C" void enable_idt(void* idtr);
extern "C" void asm_handler();

extern "C" void c_handler(uint64_t id) {
    Console::write("x"); 
}

void IDT::init() {
    idtr.limit = sizeof(idt) - 1;
    idtr.base  = (uint64_t)&idt;

    for (int i = 0; i < 256; i++) {
        set_gate(i, (uint64_t)asm_handler, 0x08, 0x8E);
    }

    enable_idt(&idtr);
}

void IDT::set_gate(int index, uint64_t addr, uint16_t selector, uint8_t flags) {
    idt[index].offset_low = (uint16_t)(addr & 0xFFFF);
    idt[index].selector = selector;
    idt[index].ist = 0;
    idt[index].type_attr = flags;
    idt[index].offset_mid = (uint16_t)((addr >> 16) & 0xFFFF);
    idt[index].offset_high = (uint32_t)((addr >> 32) & 0xFFFFFFFF);
    idt[index].zero = 0;
}