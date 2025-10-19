#include "PageTableManager.h"
#include <vector>

PageTableManager::PageTableManager(){
    this->pageTable.resize(this->pageTableSize);   
}

 PageTableManager::~PageTableManager(){

 }

 uint32_t PageTableManager::translate(uint32_t virtualAddress){
    uint8_t pageNumber = (virtualAddress >> 8) & 0xFF;
    uint8_t offset = virtualAddress & 0xFF;
    uint32_t physicalDirection = 0;
    if(isPagePresent(pageNumber)){
        physicalDirection = (pageTable[pageNumber].frameNumber * this->frameSize) + offset;
    } else {
        // TODO: pageFaultHandler.handlePageFault(pageNumber);
        std::cout << "PAGETABLEMANAGER: Direction no found" << std::endl;
        return -1;
    }

    return physicalDirection; 
 }

 void PageTableManager::setFrame(uint32_t virtualPageNumber, uint32_t _frameNumber){
    pageTable[virtualPageNumber].frameNumber = _frameNumber;
 }

 bool PageTableManager::isPagePresent(uint32_t virtualPageNumber) const {
    if (pageTable[virtualPageNumber].frameNumber > -1) {
        return true;
    } else {
        return false;
    }
 }

void PageTableManager::markModified(uint32_t virtualAddress){
    uint8_t pageNumber = (virtualAddress >> 8) & 0xFF;
    if(isPagePresent(pageNumber)){
        this->pageTable[pageNumber].dirtyBit = true;
    } else {
        
        // TODO: pageFaultHandler.handlePageFault(pageNumber);
        std::cout << "PAGETABLEMANAGER: Direction no found" << std::endl;
    }
}