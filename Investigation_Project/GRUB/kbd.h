#pragma once
/**
 * @brief Get a character from the keyboard, blocking until input is available
 * 
 * @return int ASCII code of the character (or '\n' / '\b')
 */
int kbd_getchar_blocking(void);   // retorna ASCII (o '\n' / '\b')
