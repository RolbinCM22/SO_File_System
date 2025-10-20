#pragma once
#include "PhysicalMemoryManager.h"
#include "BackingStoreManager.h"


class PageFaultHandler {
    private:
    PhysicalMemoryManager physicalMemory;
    BackingStoreManager backingStore;
    

public:
    /**
     * @brief Construct a new Page Fault Handler object 
     * 
     */
    PageFaultHandler();

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
    uint8_t handlePageFault(uint8_t pageNumber, uint8_t offset);
};