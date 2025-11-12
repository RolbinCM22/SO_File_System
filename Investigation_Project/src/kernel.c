#include <stdint.h>
#define VGA ((volatile uint16_t*)0xB8000)
#define cols 80
#define rows 25

static void cls(void){
    for(int i=0; i<cols*rows; i++){
        VGA[i] = (uint16_t) ' ' | ((uint16_t) 0x07 << 8);
    }
}

static void puts_at(int r,int c,const char* s,uint8_t attr){
    for(;*s && r<rows; ++s){
        VGA[r*cols + c] = (uint16_t)((uint8_t)*s | (attr<<8));
        if(++c>=cols){ c=0; if(++r>=rows) break; }
    }
}

void kernel_main(void){
    cls();
    puts_at(10, 30, "Investigation Project", 0x1E);
    puts_at(12, 28, "Kernel Loaded Successfully!", 0x2F);
    while(1);
}

/* Punto de entrada colocado al inicio del binario (0x00010000) */
__attribute__((section(".start"))) void start32(void){
    kernel_main();
}