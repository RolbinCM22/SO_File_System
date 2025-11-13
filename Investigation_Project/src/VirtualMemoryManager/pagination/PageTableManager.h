#pragma once
#include <vector>
#include <cstdint>
#include "../pagination/PageFaultHandler.h"
#include "../structures/PageTableEntry.h"


class PageTableManager {
    private:
        /**
         * @brief table of page table entries
         * 
         */
        pageTableEntry pageTable[256];
        /**
         * @brief Page fault handler
         *
         */
        PageFaultHandler& faultHandler;
        /**
         * @brief Size of each frame in bytes
         *
         */
        int frameSize = 256;
        /**
         * @brief Vector to track dirty pages
         *
         */
        std::vector<bool> dirtyPages = std::vector<bool>(256, false);
    public:
        /**
         * @brief Construct a new Page Table Manager object
         * 
         */
        PageTableManager(PageFaultHandler& faultHandler);
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
        void markModified(uint8_t virtualPageNumber);
   
        //   /**
        //    * @brief Mark a page as referenced
        //    * @param virtualPageNumber Virtual page number
        //    */
        //  void markReferenced(uint8_t pageNumber);

        /**
         * @brief Get the frame size used by this page table (bytes per frame)
         */
        int getFrameSize() const { return frameSize; }
        /**
         * @brief Get the list of dirty pages
         * @return std::vector<bool> Vector indicating which pages are dirty
         */
        std::vector<bool> getDirtyPages() const;
};