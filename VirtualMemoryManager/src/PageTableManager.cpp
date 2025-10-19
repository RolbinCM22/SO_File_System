#include "PageTableManager.h"
#include <vector>

PageTableManager::PageTableManager(){
    this->pageTable.resize(this->pageTableSize); 
    faultHandler = new PageFaultHandler();
    
}

 PageTableManager::~PageTableManager(){

 }
 uint8_t extractPageNumber(uint32_t virtualAddress) const {
    return (virtualAddress >> 8) & 0xFF;
}

uint8_t extractOffset(uint32_t virtualAddress) const {
    return virtualAddress & 0xFF;
}

 uint32_t PageTableManager::translate(uint32_t virtualAddress){
    uint8_t pageNumber = extractPageNumber(virtualAddress);
    uint8_t offset = extractOffset(virtualAddress);
    uint32_t physicalDirection = 0;
    if(isPagePresent(pageNumber)){
        physicalDirection = (pageTable[pageNumber].frameNumber * this->frameSize) + offset;
    } else {
        this->faultHandler.handlePageFault(pageNumber);
        std::cout << "PAGETABLEMANAGER: Direction no found" << std::endl;
        return -1;
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

void PageTableManager::markModified(uint32_t virtualAddress){
    uint8_t pageNumber = extractPageNumber(virtualAddress);
    if(isPagePresent(pageNumber)){
        this->pageTable[pageNumber].dirtyBit = true;
    } else {        
        // TODO: pageFaultHandler.handlePageFault(pageNumber);
        std::cout << "PAGETABLEMANAGER: Direction no found" << std::endl;
    }
}

