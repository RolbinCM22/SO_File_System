#include "VirtualMemoryUnit.h"

VirtualMemoryUnit::VirtualMemoryUnit(/** PageTableManager& ptm, PageFaultHandler& pfh,*/ MemoryStats& stats)
  : /** pageTableManager(ptm), pageFaultHandler(pfh),*/ stats(stats) {

}

char VirtualMemoryUnit::read_memory(size_t vpn, const size_t offset) const {
  this->validate_address(vpn, offset);
  this->stats.accesses++;

  // Simulate page table walk cost
  this->stats.page_table_walks += PAGE_TABLE_WALK_COST_LEVELS;

  this->stats.hits++;

  return static_cast<char>(vpn & 0xFF); // Return VPN as byte for testing
}

void VirtualMemoryUnit::write_memory(size_t vpn, size_t offset, char value) {
  this->validate_address(vpn, offset);
  this->stats.accesses++;

  // Simulate page table walk cost
  this->stats.page_table_walks += PAGE_TABLE_WALK_COST_LEVELS;

  this->stats.hits++;
}

void VirtualMemoryUnit::sync_dirty_pages() {
  std::cout << "VMU: Syncing dirty pages to backing store..." << std::endl;
}

void VirtualMemoryUnit::validate_address(size_t vpn, size_t offset) const {
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

char VirtualMemoryUnit::read_from_frame(const size_t frame_idx, size_t offset) {
  std::cout << "VMU: Reading page from frame :" << frame_idx << std::endl;
  return 0;
}

void VirtualMemoryUnit::write_to_frame(size_t frame_idx, size_t offset, char value) {
  std::cout << "VMU: Writing page frame : " << frame_idx << "back to logic memory" << std::endl;
}