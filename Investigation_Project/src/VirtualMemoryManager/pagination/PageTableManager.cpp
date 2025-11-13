#include "PageTableManager.h"
#include <vector>

PageTableManager::PageTableManager() : faultHandler(PageFaultHandler::instance()) {
}

PageTableManager::~PageTableManager()= default;

 uint8_t extractPageNumber(uint32_t virtualAddress) {
    return (virtualAddress >> 8) & 0xFF;
}

uint8_t extractOffset(uint32_t virtualAddress) {
    return virtualAddress & 0xFF;
}

 uint32_t PageTableManager::translate(uint32_t virtualAddress){
    uint8_t pageNumber = extractPageNumber(virtualAddress);
    uint8_t offset = extractOffset(virtualAddress);
    uint32_t physicalDirection = 0;
    if(isPagePresent(pageNumber)){
        physicalDirection = (pageTable[pageNumber].frameNumber * this->frameSize) + offset;
    } else {
        uint8_t frame = this->faultHandler.handlePageFault(pageNumber, offset);
        setFrame(pageNumber, frame); 
        std::cout << " PAGETABLEMANAGER: Direction no found" << std::endl;
        physicalDirection = (pageTable[pageNumber].frameNumber * this->frameSize) + offset;
        std::cout << " PAGETABLEMANAGER: Direction found: " << physicalDirection << std::endl;
        return physicalDirection;
    }

    return physicalDirection; 
 }

 void PageTableManager::setFrame(uint32_t virtualPageNumber, uint32_t _frameNumber){
    pageTable[virtualPageNumber].frameNumber = _frameNumber;
    pageTable[virtualPageNumber].present = true;
    pageTable[virtualPageNumber].dirtyBit = false;
    
 }

 bool PageTableManager::isPagePresent(uint32_t virtualPageNumber) const {
   return pageTable[virtualPageNumber].present;
 }

void PageTableManager::markModified(uint8_t pageNumber){
    this->pageTable[pageNumber].dirtyBit = true;
    this->dirtyPages[pageNumber] = true;
}

std::vector<bool> PageTableManager::getDirtyPages() const {
    return dirtyPages;
}


