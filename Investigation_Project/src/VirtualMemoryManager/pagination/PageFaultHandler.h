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
   * @brief Returns the singleton instance of the PageFaultHandler.
   *
   * This method provides access to the single shared instance of the
   * PageFaultHandler class. It ensures that only one instance exists
   * throughout the system by creating it the first time it is called
   * and returning the same reference on subsequent calls.
   *
   * @return Reference to the singleton PageFaultHandler instance.
   */
    static PageFaultHandler& instance() {
      static PageFaultHandler instance;
      return instance;
    }

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