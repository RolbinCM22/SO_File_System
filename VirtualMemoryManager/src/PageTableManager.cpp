#include "PageTableManager.h"


PageTableManager::PageTableManager(){
    this->pageTable.resize(this->pageTableSize);   
}

 PageTableManager::~PageTableManager(){

 }

 uint32_t PageTableManager::translate(uint32_t virtualAddress){
    uint8_t pageNumber = (virtualAddress >> 8) & 0xFF;
    uint8_t offset = virtualAddress & 0xFF;
    uint32_t physicalDirection = 0;
    if(pageTable[pageNumber].frameNumber > -1){
        physicalDirection = (pageTable[pageNumber].frameNumber * this->frameSize) + offset;
    } else {
        // TODO: pageFaultHandler.handlePageFault(pageNumber);
        std::cout << "PAGETABLEMANAGER: Direction no found" << std::endl;
        return -1;
    }

    return physicalDirection; 
 }