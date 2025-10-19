#pragma once
#include "PhysicalMemoryManager.h"
#include "BackingStoreManager.h"
#include "PageTableManager.h"

class PageFaultHandler {
    private:
    PhysicalMemoryManager* physicalMemory;
    BackingStoreManager* backingStore;
    

public:
    /**
     * @brief Construct a new Page Fault Handler object 
     * 
     * @param pm  Physical memory manager
     * @param bs  Backing store manager
     */
    PageFaultHandler(PhysicalMemoryManager* pm,
                     BackingStoreManager* bs);

    /**
     * @brief Destroy the Page Fault Handler object
     * 
     */
    ~PageFaultHandler();

    /**
     * @brief Handle a page fault
     * 
     * @param pageNumber The page number that caused the fault
     */
    void handlePageFault(uint32_t pageNumber);
};