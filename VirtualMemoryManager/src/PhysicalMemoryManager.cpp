#include "PhysicalMemoryManager.h"
PhysicalMemoryManager::PhysicalMemoryManager() {
    // Initialize physical memory and frame usage
    for (auto& frame : physicalMemory) {
        frame.fill(0);
    }
    frameUsage.fill(false);
    frameToPage.fill(std::nullopt);
}
size_t PhysicalMemoryManager::allocate_frame() {
    // Search for a free frame
    for (size_t i = 0; i < NUM_FRAMES; ++i) {
        if (!frameUsage[i]) {
            frameUsage[i] = true;
            return i;
        }
    }
    // No free frame found, use FIFO replacement
    size_t frameIndex = nextFrameToReplace;
    nextFrameToReplace = (nextFrameToReplace + 1) % NUM_FRAMES;
    return frameIndex;
}
void PhysicalMemoryManager::free_frame(size_t frameIndex) {
    if (frameIndex >= NUM_FRAMES) {
        throw std::out_of_range("Frame index out of range");
    }
    frameUsage[frameIndex] = false;
    frameToPage[frameIndex] = std::nullopt;
}
char PhysicalMemoryManager::read_byte(size_t frameIndex, size_t offset) const {
    if (frameIndex >= NUM_FRAMES || offset >= PAGE_SIZE) {
        throw std::out_of_range("Frame index or offset out of range");
    }
    return physicalMemory[frameIndex][offset];
}
void PhysicalMemoryManager::write_byte(size_t frameIndex, size_t offset, char value) {
    if (frameIndex >= NUM_FRAMES || offset >= PAGE_SIZE) {
        throw std::out_of_range("Frame index or offset out of range");
    }
    physicalMemory[frameIndex][offset] = value;
}
std::optional<size_t> PhysicalMemoryManager::get_page_in_frame(size_t frameIndex) const {
    if (frameIndex >= NUM_FRAMES) {
        throw std::out_of_range("Frame index out of range");
    }
    return frameToPage[frameIndex];
}
void PhysicalMemoryManager::set_frame_mapping(size_t frameIndex, size_t vpn) {
    if (frameIndex >= NUM_FRAMES) {
        throw std::out_of_range("Frame index out of range");
    }
    frameToPage[frameIndex] = vpn;
}
void PhysicalMemoryManager::print_frame_table() const {
    std::cout << "Frame Table:\n";
    std::cout << "Frame Index | In Use | Virtual Page\n";
    std::cout << "-------------------------------------\n";
    for (size_t i = 0; i < NUM_FRAMES; ++i) {
        std::cout << std::setw(11) << i << " | "
                  << std::setw(6) << (frameUsage[i] ? "Yes" : "No") << " | ";
        if (frameToPage[i].has_value()) {
            std::cout << std::setw(12) << frameToPage[i].value() << "\n";
        } else {
            std::cout << std::setw(12) << "N/A" << "\n";
        }
    }
}   