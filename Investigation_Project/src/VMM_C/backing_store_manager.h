#pragma once
#include <stddef.h>
#include <stdint.h>
#include "constants.h"

typedef struct backing_store_manager {
    char store[BACKING_PAGES][PAGE_SIZE];
} backing_store_manager_t;

void bsm_init(backing_store_manager_t* bsm);

void bsm_load_page(const backing_store_manager_t* bsm, size_t vpn, char* out_buf);     // PAGE_SIZE
void bsm_save_page(backing_store_manager_t* bsm, size_t vpn, const char* in_buf);      // PAGE_SIZE

void bsm_init_test_patterns(backing_store_manager_t* bsm);

char bsm_read_byte(const backing_store_manager_t* bsm, size_t vpn, size_t offset);
void bsm_write_byte(backing_store_manager_t* bsm, size_t vpn, size_t offset, char value);
