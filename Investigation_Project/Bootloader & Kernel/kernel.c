#include <stdint.h>
#define VGA ((volatile uint16_t*)0xB8000)
#define cols 80
#define rows 25
/**
 * @brief   Limpia la pantalla VGA estableciendo todos los caracteres a espacios en blanco con atributo de color predeterminado.
 * 
 */
static void cls(void){
    for(int i=0; i<cols*rows; i++){
        VGA[i] = (uint16_t) ' ' | ((uint16_t) 0x07 << 8);
    }
}
/**
 * @brief  Escribe una cadena de texto en la posición (r, c) con un atributo de color.
 * 
 * @param r Fila donde se escribirá el texto.
 * @param c Columna donde se escribirá el texto.
 * @param s Puntero a la cadena de texto.
 * @param attr Atributo de color del texto.
 */
static void puts_at(int r,int c,const char* s,uint8_t attr){
    for(;*s && r<rows; ++s){
        VGA[r*cols + c] = (uint16_t)((uint8_t)*s | (attr<<8));
        if(++c>=cols){ c=0; if(++r>=rows) break; }
    }
}

/**
 * @brief Núcleo principal del sistema operativo.
 * 
 */
void kernel_main(void){
    cls();
    puts_at(2, 30, "Bienvenido al MichiKernel", 0x1E);
    puts_at(3, 35, "Instrucciones", 0x2F);
    puts_at(5, 30, "Kernel de prueba con un michi", 0x0A); // ← 38 chars, no se desborda

    puts_at(15, 32,  "     |\\__/,|   (`\\", 0x0F);
    puts_at(16, 32, "   _.|o o  |_   ) )", 0x0F);
    puts_at(17, 32, "---(((---(((--------", 0x0F);
    puts_at(19, 33, "     Meow :3", 0x0E);
    
    while(1);
}

/* Punto de entrada colocado al inicio del binario (0x00010000) */
__attribute__((section(".start"))) void start32(void){
    kernel_main();
}