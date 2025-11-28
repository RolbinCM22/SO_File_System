#pragma once
#include <stdint.h>

void cls(void);
void puts_at(int row, int col, const char* s, uint8_t color);
int  printf(const char* fmt, ...);

/**
 * @brief Set the color of the console text
 * 
 * @param color 
 */
void console_set_color(uint8_t color);
/**
 * @brief Print a character to the console
 * 
 * @param c 
 */
void console_putc(char c);
/**
 * @brief Print a string to the console
 * 
 * @param s 
 */
void console_write(const char* s);
/**
 * @brief Print a string followed by a newline to the console
 * 
 * @param s 
 */
void console_writeln(const char* s);
