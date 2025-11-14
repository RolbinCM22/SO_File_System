
#include "PageFaultHandler.h"

#include <cstdint>
#include <iostream>

#include "../constans.h"

PageFaultHandler::PageFaultHandler(PhysicalMemoryManager& mem, BackingStoreManager& bsm)
  : physicalMemory(mem)
  , backingStore(bsm) {

}

PageFaultHandler::~PageFaultHandler() = default;

uint8_t PageFaultHandler::handlePageFault(uint8_t pageNumber, uint8_t offset){
    // Search empty frame
    size_t frame = physicalMemory.allocate_frame();
    // copy data of backing store of buffer
    char buffer[PAGE_SIZE];
    backingStore.load_page(pageNumber, buffer);

    physicalMemory.write_byte(frame, offset, *buffer);
    std::cout << "handlePageFault: Frame asign: " << frame;
    return static_cast<uint8_t>(frame);
}