#pragma once
#include <stdint.h>

struct page_table_manager;          
typedef struct page_table_manager page_table_manager_t;

typedef struct physical_memory_manager physical_memory_manager_t;
typedef struct backing_store_manager backing_store_manager_t;

typedef struct {
    physical_memory_manager_t* pmm;
    backing_store_manager_t* bsm;
} page_fault_handler_t;

/* init */
void pfh_init(page_fault_handler_t* pfh,
              physical_memory_manager_t* pmm,
              backing_store_manager_t* bsm);

/* Maneja fault de vpn: asigna frame, carga página completa, actualiza PTM */
int pfh_handle_page_fault(page_fault_handler_t* pfh,
                          page_table_manager_t* ptm,
                          uint8_t vpn);
