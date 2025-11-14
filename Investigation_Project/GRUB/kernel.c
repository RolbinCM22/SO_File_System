#include <stdint.h>
#define VIDEO ((volatile char*)0xB8000) // Dirección base de la memoria de video VGA en modo texto
/**
 * @brief Imprime una cadena en la pantalla VGA en la posición (row, col) con el color especificado.
 * 
 * @param s  Puntero a la cadena de texto.
 * @param color Color del texto.
 * @param row Fila donde se imprimirá el texto.
 * @param col Columna donde se imprimirá el texto.
 */
void print(const char* s, uint8_t color, int row, int col) {
    int pos = (row * 80 + col) * 2;
    while (*s) {
        VIDEO[pos++] = *s++;
        VIDEO[pos++] = color;
    }
}

/**
 * @brief Núcleo principal del sistema operativo.
 * 
 */
void kernel_main() {
    print("Hola desde MichiKernel v2 (con GRUB!)", 0x0F, 10, 20);
    print("System calls activadas", 0x0E, 12, 25);

    while (1) __asm__ volatile ("hlt"); // Espera en un bucle infinito
}