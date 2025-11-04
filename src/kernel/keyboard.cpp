#include <stdint.h>
#include <console.hpp>

static bool caps1 = false;
static bool caps2 = false;

static const char lowercase[128] = {
'\0','\0','1','2','3','4','5','6','7','8','9','0','-','=','\b','\t','q','w','e','r',
't','y','u','i','o','p','[',']','\n','\0','a','s','d','f','g','h','j','k','l',';',
'\'','`','\0','\\','z','x','c','v','b','n','m',',','.','/','\0','*','\0',' ','\0',
'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','-','\0',
'\0','\0','+'
};

static const char uppercase[128] = {
'\0','\0','!','@','#','$','%','^','&','*','(',')','_','+','\b','\t','Q','W','E','R',
'T','Y','U','I','O','P','{','}','\n','\0','A','S','D','F','G','H','J','K','L',':','"',
'~','\0','|','Z','X','C','V','B','N','M','<','>','?','\0','*','\0',' ','\0','\0',
'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','\0','-','\0','\0','\0','+'
};

void keyboard_handler(uint8_t scancode) {
    char keycode = scancode & 0x7F;
    bool pressed = (scancode & 0x80) == 0;

    if (keycode == 0x2A || keycode == 0x36) {
        caps2 = pressed;
        return;
    }

    if (!pressed)
        return;

    if (keycode == 0x3A) {
        caps1 = !caps1;
        return;
    }
    if (keycode == 0x0E) {
        // Console::erase();
        return;
    }

    char key = (caps1 || caps2) ? uppercase[keycode] : lowercase[keycode];
    if (key) {
        char buff[] = {key, '\0'};
        Console::write(buff);
    }
}