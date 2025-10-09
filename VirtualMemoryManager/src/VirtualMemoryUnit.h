#ifndef VIRTUALMEMORYMANAGER_VIRTUALMEMORYUNIT_H
#define VIRTUALMEMORYMANAGER_VIRTUALMEMORYUNIT_H
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

#include "structures/MemoryStats.h"

// Configuration constants
static constexpr size_t PAGE_SIZE = 4096;           ///< Page size in bytes
static constexpr size_t NUM_FRAMES = 4;             ///< Physical frames
static constexpr size_t BACKING_PAGES = 64;         ///< Virtual pages
static constexpr size_t PAGE_TABLE_WALK_COST_LEVELS = 2;  ///< Necessary indirections to reach page on logic memory.

/**
 * @brief Memory Management Unit - handles virtual to physical address translation
 */
class VirtualMemoryUnit {
private:
    // PageTableManager& pageTableManager;
    // PageFaultHandler& pageFaultHandler;
    MemoryStats& stats;

public:
    /**
     * @brief Construct a new MMU object
     * @param stats Statistics collector reference
     */
    explicit VirtualMemoryUnit(/** PageTableManager& ptm, PageFaultHandler& pfh,*/ MemoryStats& stats);

    /**
     * @brief Read a byte from virtual memory
     * @param vpn Virtual page number
     * @param offset Byte offset within page
     * @return char The read byte value
     * @throws std::out_of_range if vpn or offset are invalid
     */
    char read_memory(size_t vpn, const size_t offset) const;

    /**
     * @brief Write a byte to virtual memory
     *
     * @param vpn Virtual page number
     * @param offset Byte offset within page
     * @param value Byte value to write
     * @throws std::out_of_range if vpn or offset are invalid
     */
    void write_memory(size_t vpn, size_t offset, char value);

    /**
     * @brief Sync all dirty pages to backing store
     */
    void sync_dirty_pages();

    /**
     * @brief Get memory statistics
     * @return const MemoryStats& Reference to statistics
     */
    [[nodiscard]] const MemoryStats& get_stats() const { return this->stats; }

private:
  /**
   * @brief Validate virtual address components
   * @param vpn Virtual page number
   * @param offset Byte offset within page
   * @throws std::out_of_range if address is invalid
   */
  void validate_address(size_t vpn, size_t offset) const;

  /**
   * @brief Read a byte from a physical frame
   * @param frame_idx Physical frame index
   * @param offset Byte offset within frame
   * @return char The read byte value
   */
  char read_from_frame(size_t frame_idx, size_t offset);

  /**
   * @brief Write a byte to a physical frame
   * @param frame_idx Physical frame index
   * @param offset Byte offset within frame
   * @param value Byte value to write
   */
  void write_to_frame(size_t frame_idx, size_t offset, char value);
};

#endif //VIRTUALMEMORYMANAGER_VIRTUALMEMORYUNIT_H