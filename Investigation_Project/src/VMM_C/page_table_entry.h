#pragma once
#include <stdint.h>

typedef struct {
    uint8_t frame;      
    uint8_t present;    
    uint8_t dirty;     
    uint8_t accessed;   
} pte_t;
