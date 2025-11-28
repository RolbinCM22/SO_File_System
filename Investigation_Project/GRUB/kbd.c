#include "kbd.h"
#include <stdint.h>

static inline uint8_t inb(uint16_t port){
    uint8_t ret;
    __asm__ volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

static int shift_down = 0;

static const char map[128] = {
    0,  27,'1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n', 0,
    'a','s','d','f','g','h','j','k','l',';','\'','`', 0, '\\',
    'z','x','c','v','b','n','m',',','.','/', 0, '*', 0, ' ',
};

static const char map_shift[128] = {
    0,  27,'!','@','#','$','%','^','&','*','(',')','_','+', '\b',
    '\t','Q','W','E','R','T','Y','U','I','O','P','{','}','\n', 0,
    'A','S','D','F','G','H','J','K','L',':','"','~', 0, '|',
    'Z','X','C','V','B','N','M','<','>','?', 0, '*', 0, ' ',
};

static int scancode_to_ascii(uint8_t sc){
    // shift press/release
    if(sc == 0x2A || sc == 0x36) { shift_down = 1; return 0; }
    if(sc == 0xAA || sc == 0xB6) { shift_down = 0; return 0; }

    // ignore releases (>= 0x80)
    if(sc & 0x80) return 0;

    if(sc < 128){
        char ch = shift_down ? map_shift[sc] : map[sc];
        return (int)ch;
    }
    return 0;
}

int kbd_getchar_blocking(void){
    while(1){
        uint8_t status = inb(0x64);
        if(status & 0x01){
            uint8_t sc = inb(0x60);
            int ch = scancode_to_ascii(sc);
            if(ch) return ch;
        }
    }
}
