#pragma once
#include <stdint.h>
#include <stddef.h>
#include "constants.h"

typedef struct physical_memory_manager {
    char mem[NUM_FRAMES][PAGE_SIZE];
    uint8_t in_use[NUM_FRAMES];
    int16_t frame_to_page[NUM_FRAMES];
    uint8_t next_victim;
} physical_memory_manager_t;


void pmm_init(physical_memory_manager_t* pmm);


int  pmm_allocate_frame(physical_memory_manager_t* pmm);

void pmm_free_frame(physical_memory_manager_t* pmm, size_t frame);

char pmm_read_byte(const physical_memory_manager_t* pmm, size_t frame, size_t offset);

void pmm_write_byte(physical_memory_manager_t* pmm, size_t frame, size_t offset, char value);

void pmm_read_frame(const physical_memory_manager_t* pmm, size_t frame, char* out_buf, size_t size);

void pmm_write_frame(physical_memory_manager_t* pmm, size_t frame, const char* in_buf, size_t size);

/* mapping */
int  pmm_get_page_in_frame(const physical_memory_manager_t* pmm, size_t frame); // vpn o -1
void pmm_set_frame_mapping(physical_memory_manager_t* pmm, size_t frame, int vpn);

void pmm_print_frame_table(const physical_memory_manager_t* pmm);
