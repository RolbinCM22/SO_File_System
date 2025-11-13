#ifndef VIRTUALMEMORYMANAGER_VIRTUALMEMORYUNIT_H
#define VIRTUALMEMORYMANAGER_VIRTUALMEMORYUNIT_H
#include <iomanip>
#include <stdexcept>

#include "pagination/PageFaultHandler.h"
#include "pagination/PageTableManager.h"
#include "physical/PhysicalMemoryManager.h"
#include "structures/MemoryStats.h"

/**
 * @brief Memory Management Unit - handles virtual to physical address translation
 */
class VirtualMemoryUnit {
private:
    PageTableManager& pageTableManager;
    PageFaultHandler& pageFaultHandler;
    PhysicalMemoryManager& physManager;
    MemoryStats stats;

public:
    /**
     * @brief Construct a new MMU object
     */
    explicit VirtualMemoryUnit(PageTableManager& ptm);

    /**
     * @brief Read a byte from virtual memory
     * @param vpn Virtual page number
     * @param offset Byte offset within page
     * @return char The read byte value
     * @throws std::out_of_range if vpn or offset are invalid
     */
    char read_memory(size_t vpn, size_t offset);

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
  [[nodiscard]] char read_from_frame(size_t frame_idx, size_t offset) const;

  /**
   * @brief Write a byte to a physical frame
   * @param frame_idx Physical frame index
   * @param offset Byte offset within frame
   * @param value Byte value to write
   */
  void write_to_frame(size_t frame_idx, size_t offset, char value) const;
};

#endif //VIRTUALMEMORYMANAGER_VIRTUALMEMORYUNIT_H