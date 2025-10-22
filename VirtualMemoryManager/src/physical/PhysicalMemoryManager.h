#ifndef PHYSICALMEMORYMANAGER_H
#define PHYSICALMEMORYMANAGER_H

#include <array>
#include <optional>
#include <iostream>

#include "../VirtualMemoryUnit.h"

/**
 * @brief Manages physical memory frames allocation and replacement
 */
class PhysicalMemoryManager {
private:
    /// Simulated physical memory: NUM_FRAMES × PAGE_SIZE bytes
    std::array<std::array<char, PAGE_SIZE>, NUM_FRAMES> physicalMemory{};

    /// Tracks which frames are occupied (true = in use)
    std::array<bool, NUM_FRAMES> frameUsage{};

    /// Tracks which virtual page is currently in each frame
    std::array<std::optional<size_t>, NUM_FRAMES> frameToPage{};

    /// Current frame index for FIFO replacement
    size_t nextFrameToReplace = 0;

  /**
   * @brief Construct a new Physical Memory Manager object
   */
    PhysicalMemoryManager();

public:

    static PhysicalMemoryManager& instance() {
      static PhysicalMemoryManager instance;
      return instance;
    }

    /**
     * @brief Allocate a free frame, or choose one for replacement if full
     * @return size_t Frame index allocated
     */
    size_t allocate_frame();

    /**
     * @brief Free a specific frame
     * @param frameIndex Index of the frame to release
     */
    void free_frame(size_t frameIndex);

    /**
     * @brief Read a byte from physical memory
     * @param frameIndex Physical frame index
     * @param offset Byte offset within the frame
     * @return char The value read
     */
    char read_byte(size_t frameIndex, size_t offset) const;

    /**
     * @brief Write a byte to physical memory
     * @param frameIndex Physical frame index
     * @param offset Byte offset within the frame
     * @param value Byte to write
     */
    void write_byte(size_t frameIndex, size_t offset, char value);

    void read_frame(size_t frameIndex, char *outBuffer, size_t size) const;

    void write_frame(size_t frameIndex, const char *inBuffer, size_t size);

    /**
     * @brief Get the virtual page number currently stored in a frame
     * @param frameIndex Index of the frame
     * @return std::optional<size_t> Virtual page number if occupied
     */
    [[nodiscard]] std::optional<size_t> get_page_in_frame(size_t frameIndex) const;

    /**
     * @brief Link a frame to a specific virtual page
     * @param frameIndex Index of frame
     * @param vpn Virtual page number
     */
    void set_frame_mapping(size_t frameIndex, size_t vpn);

    /**
     * @brief Print current physical memory frame usage
     */
    void print_frame_table() const;
};

#endif // PHYSICALMEMORYMANAGER_H