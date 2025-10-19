#pragma once
#include <vector>
#include <cstdint>
#include "PageFaultHandler.h"


struct pageTableEntry{
    int8_t frameNumber = -1;
    bool dirtyBit;
    bool chargeInMemory;
};


class PageTableManager {
    private:
        /**
         * @brief table of page table entries
         * 
         */
        std::vector<pageTableEntry> pageTable;
        /**
         * @brief Page fault handler
         * 
         */
        PageFaultHandler* faultHandler;
        /**
         * @brief 
         * TODO: DOCUMENTAR
         */
        int pageTableSize = 256;
        int frameSize = 256;
    public:
        /**
         * @brief Construct a new Page Table Manager object
         * 
         */
        PageTableManager();
        /**
         * @brief Destroy the Page Table Manager object
         * 
         */
        ~PageTableManager();
        /**
         * @brief Translate a virtual address to a physical address
         * @param virtualAddress 
         * @return uint32_t  Physical address
         */
        uint32_t translate(uint32_t virtualAddress);
        /**
         * @brief Set the frame number for a given virtual page number
         * @param virtualPageNumber  Virtual page number
         * @param frameNumber  Frame number
         */
        void setFrame(uint32_t virtualPageNumber, uint32_t frameNumber);
        /**
         * @brief Check if a page is present in the page table
         * 
         * @param virtualPageNumber Virtual page number
         * @return true  Page is present
         * @return false  Page is not present
         */
        bool isPagePresent(uint32_t virtualPageNumber) const;
        /**
         * @brief Mark a page as modified
         * 
         * @param virtualPageNumber Virtual page number
         */
        void markModified(uint32_t virtualPageNumber);
        /**
         * @brief Mark a page as referenced
         * @param virtualPageNumber Virtual page number
         */
        void markReferenced(uint32_t virtualPageNumber);

        bool char
};