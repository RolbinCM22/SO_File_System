#pragma once
#include <stdint.h>
#include <stddef.h>

#include "page_table_entry.h"
#include "page_fault_handler.h"

#define PT_PAGES 256

typedef struct page_table_manager {
    pte_t page_table[PT_PAGES];
    page_fault_handler_t* fault_handler;
    uint16_t frame_size;
    uint8_t dirty[PT_PAGES];
} page_table_manager_t;


void ptm_init(page_table_manager_t* ptm, page_fault_handler_t* pfh);

uint32_t ptm_translate(page_table_manager_t* ptm, uint32_t virtual_address);

void ptm_set_frame(page_table_manager_t* ptm, size_t vpn, uint32_t frame);

int  ptm_is_page_present(const page_table_manager_t* ptm, size_t vpn);

void ptm_mark_modified(page_table_manager_t* ptm, size_t vpn);

uint16_t ptm_get_frame_size(const page_table_manager_t* ptm);

void ptm_get_dirty_pages(const page_table_manager_t* ptm, uint8_t out_dirty[PT_PAGES]);

int  ptm_is_dirty(const page_table_manager_t* ptm, uint32_t vpn);
void ptm_clear_dirty(page_table_manager_t* ptm, uint32_t vpn);
void ptm_invalidate(page_table_manager_t* ptm, uint32_t vpn);
