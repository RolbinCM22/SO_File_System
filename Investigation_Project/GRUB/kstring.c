#include <stddef.h>

size_t strlen(const char* s){ size_t n=0; while(s && s[n]) n++; return n; }
/**
 * @brief Compare two strings
 * 
 * @param a First string
 * @param b Second string
 * @return int Result of comparison
 */
int strcmp(const char* a, const char* b){
    while(*a && (*a==*b)){ a++; b++; }
    return (unsigned char)*a - (unsigned char)*b;
}
/**
 * @brief Copy a string with a maximum length
 * 
 * @param dst Destination buffer
 * @param src Source string
 * @param n Maximum number of characters to copy
 * @return char* Pointer to the destination buffer
 */
char* strncpy(char* dst, const char* src, size_t n){
    size_t i=0;
    for(; i<n && src && src[i]; i++) dst[i]=src[i];
    for(; i<n; i++) dst[i]='\0';
    return dst;
}
