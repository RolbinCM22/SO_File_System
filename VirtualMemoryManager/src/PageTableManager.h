#pragma once
#include <vector>
#include <cstdint>
#include "pagetableentry.h"
#include "pagefaulthandler.h"

class PageTableManager {
    private:
        std::vector<PageTableEntry> pageTable;
        PageFaultHandler* faultHandler;
    public:
        PageTableManager(size_t numPages, PageFaultHandler* handler);
        ~PageTableManager();

        uint32_t translate(uint32_t virtualAddress);

        void setFrame(uint32_t virtualPageNumber, uint32_t frameNumber);
        bool isPagePresent(uint32_t virtualPageNumber) const;
        void markModified(uint32_t virtualPageNumber);
        void markReferenced(uint32_t virtualPageNumber);
}