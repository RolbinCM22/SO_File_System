//
// Created by Aaroncz on 8/10/2025.
//

#ifndef VIRTUALMEMORYMANAGER_MEMORYSTATS_H
#define VIRTUALMEMORYMANAGER_MEMORYSTATS_H

/**
 * @brief Statistics collector for memory operations
 */
struct MemoryStats {
  size_t accesses{};           ///< Total memory accesses (reads + writes)
  size_t hits{};               ///< Accesses that found page resident
  size_t page_faults{};        ///< Number of page faults
  size_t swap_ins{};           ///< Number of swap-ins from backing store
  size_t swap_outs{};          ///< Number of swap-outs to backing store
  size_t page_table_walks{};   ///< Simulated page-table memory accesses

  /**
   * @brief Construct a new MemoryStats object with default values.
   */
  MemoryStats() = default;
};

#endif //VIRTUALMEMORYMANAGER_MEMORYSTATS_H