#include <idt.hpp>

IDT::IDTEntry IDT::idt[256];
IDT::IDTR IDT::idtr;

extern "C" void enable_idt(void* idtr);

extern "C" void asm_handler32();
extern "C" void asm_handler33();

extern "C" void outb(uint8_t value, uint16_t port);
extern "C" uint8_t inb(uint16_t port);

extern void keyboard_handler(uint8_t scancode);

extern "C" void c_handler(uint8_t id) {
    switch (id) {
        case 32:
            // TODO: Add timer driver
            break;
        case 33:
            // TODO: Add keyboard driver
            uint8_t scancode = inb(0x60);
            keyboard_handler(scancode);
            break;
    }
}

void IDT::init() {
    idtr.limit = sizeof(idt) - 1;
    idtr.base  = (uint64_t)&idt;

    set_gate(32, (uint64_t)asm_handler32, 0x08, 0x8E);
    set_gate(33, (uint64_t)asm_handler33, 0x08, 0x8E);

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