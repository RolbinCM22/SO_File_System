#pragma once
#include <stddef.h>
#include <stdint.h>

#include "constants.h"
#include "page_table_manager.h"
#include "physical_memory_manager.h"
#include "backing_store_manager.h"
#include "page_fault_handler.h"

typedef enum {
  VMU_OK = 0,
  VMU_ERR_VPN_RANGE   = -1,
  VMU_ERR_OFF_RANGE   = -2,
  VMU_ERR_FRAME_RANGE = -3
} vmu_status_t;

typedef struct {
  page_table_manager_t ptm;
  page_fault_handler_t pfh;
  physical_memory_manager_t* pmm;
  backing_store_manager_t* bsm;
} virtual_memory_unit_t;
/**
 * @brief   Initialize the Virtual Memory Unit
 * 
 * @param vmu   
 * @param pmm 
 * @param bsm 
 */
void vmu_init(virtual_memory_unit_t* vmu,
              physical_memory_manager_t* pmm,
              backing_store_manager_t* bsm);

/**
 * @brief   Read a byte from virtual memory
 * 
 * @param vmu 
 * @param vpn       
 * @param offset 
 * @param out_value 
 * @return vmu_status_t 
 */
vmu_status_t vmu_read_memory(virtual_memory_unit_t* vmu,
                             size_t vpn, size_t offset,
                             char* out_value);

/**
 * @brief   Write a byte to virtual memory
 * 
 * @param vmu 
 * @param vpn 
 * @param offset 
 * @param value 
 * @return vmu_status_t 
 */
vmu_status_t vmu_write_memory(virtual_memory_unit_t* vmu,
                              size_t vpn, size_t offset,
                              char value);

/**
 * @brief   Write all dirty pages back to the backing store
 * 
 * @param vmu 
 */
void vmu_sync_dirty_pages(virtual_memory_unit_t* vmu);
