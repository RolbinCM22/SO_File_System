#pragma once

#include <cstdint>

#include "../physical/PhysicalMemoryManager.h"
#include "../physical/BackingStoreManager.h"

class PageFaultHandler {
    private:
    PhysicalMemoryManager& physicalMemory;
    BackingStoreManager& backingStore;

public:

    /**
     * @brief Construct a new Page Fault Handler object
     *
     */
    PageFaultHandler(PhysicalMemoryManager& mem, BackingStoreManager& bsm);

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