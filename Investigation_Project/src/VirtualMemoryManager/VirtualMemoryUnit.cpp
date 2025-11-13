#include "VirtualMemoryUnit.h"

#include "constans.h"

VirtualMemoryUnit::VirtualMemoryUnit(
  PageTableManager& ptm, PageFaultHandler& pfh, PhysicalMemoryManager& physmm)
  : pageTableManager(ptm)
  , pageFaultHandler(pfh)
  , physManager(physmm)
  , stats(MemoryStats()) {

}

char VirtualMemoryUnit::read_memory(const size_t vpn, const size_t offset) {
  this->validate_address(vpn, offset);
  this->stats.accesses++;

  // Simulate page table walk cost
  this->stats.page_table_walks += PAGE_TABLE_WALK_COST_LEVELS;

  // Build virtual address using the existing page/offset layout (8-bit page + 8-bit offset)
  const auto virtualAddress = static_cast<uint32_t>((vpn << 8) | (offset & 0xFFu));
  const uint32_t physicalAddress = this->pageTableManager.translate(virtualAddress);
  //
  // Convert physicalAddress to frame index and frame offset using PTM's frame size
  const int frameSize = this->pageTableManager.getFrameSize();
  const auto frameIdx = static_cast<size_t>(physicalAddress / static_cast<uint32_t>(frameSize));
  const auto frameOffset = static_cast<size_t>(physicalAddress % static_cast<uint32_t>(frameSize));

  // Read data from physical memory via PFH accessor
  const char value = this->read_from_frame(frameIdx, frameOffset);

  this->stats.hits++;
  return value;
}

void VirtualMemoryUnit::write_memory(const size_t vpn, const size_t offset, char value) {
  this->validate_address(vpn, offset);
  this->stats.accesses++;

  // Simulate page table walk cost
  this->stats.page_table_walks += PAGE_TABLE_WALK_COST_LEVELS;

  // Build virtual address using the existing page.
  const auto virtualAddress = static_cast<uint32_t>((vpn << 8) | (offset & 0xFFu));
  const uint32_t physicalAddress = this->pageTableManager.translate(virtualAddress);

  // Convert physicalAddress to frame index and frame offset.
  const int frameSize = this->pageTableManager.getFrameSize();
  const auto frameIdx = static_cast<size_t>(physicalAddress / static_cast<uint32_t>(frameSize));
  const auto frameOffset = static_cast<size_t>(physicalAddress % static_cast<uint32_t>(frameSize));

  // Writes out the data to the physical memory.
  this->write_to_frame(frameIdx, frameOffset, value);

  // Mark the PageTableEntry as dirty.
  this->pageTableManager.markModified(static_cast<uint8_t>(vpn & 0xFFu));

  this->stats.hits++;
}

void VirtualMemoryUnit::sync_dirty_pages() {
  std::cout << "VMU: Syncing dirty pages to backing store..." << std::endl;
  for (size_t vpn = 0; vpn < 256; ++vpn) {
    // Verifies if the pages is loaded.
    if (this->pageTableManager.isPagePresent(static_cast<uint32_t>(vpn))) {
      // Build virtual address using the existing page.
      uint32_t virtualAddress = static_cast<uint32_t>((vpn << 8) | 0u);
      uint32_t physicalAddress = this->pageTableManager.translate(virtualAddress);
      int frameSize = this->pageTableManager.getFrameSize();
      size_t frameIdx = static_cast<size_t>(physicalAddress / static_cast<uint32_t>(frameSize));


    }
  }
  std::cout << "VMU: Sync complete." << std::endl;
}

void VirtualMemoryUnit::validate_address(const size_t vpn, const size_t offset) const {
  if (vpn >= BACKING_PAGES) {
    throw std::out_of_range(
      "VMU: Virtual page number " + std::to_string(vpn) + " out of range"
      );
  }
  if (offset >= PAGE_SIZE) {
    throw std::out_of_range(
      "VMU: Offset " + std::to_string(offset) + " exceeds page size"
      );
  }
}

char VirtualMemoryUnit::read_from_frame(const size_t frame_idx, const size_t offset) const {
  if (frame_idx >= NUM_FRAMES) {
    throw std::out_of_range("VMU: Frame index out of range");
  }
  if (offset >= PAGE_SIZE) {
    throw std::out_of_range("VMU: Offset out of range within frame");
  }

  // Read one byte from the physical frame
  const char value = this->physManager.read_byte(frame_idx, offset);

  std::cout << "[VMU] Read from frame=" << frame_idx
            << " offset=" << offset
            << " value=" << static_cast<int>(value)
            << std::endl;

  return value;
}

void VirtualMemoryUnit::write_to_frame(const size_t frame_idx, const size_t offset, const char value) const {
  if (frame_idx >= NUM_FRAMES) {
    throw std::out_of_range("VMU: Frame index out of range");
  }

  if (offset >= PAGE_SIZE) {
    throw std::out_of_range("VMU: Offset out of range within frame");
  }

  // Write one byte to the physical frame
  this->physManager.write_byte(frame_idx, offset, value);

  std::cout << "[VMU] Write to frame=" << frame_idx
            << " offset=" << offset
            << " value=" << static_cast<int>(value)
            << std::endl;
}
