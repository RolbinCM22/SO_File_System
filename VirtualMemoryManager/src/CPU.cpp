#include "CPU.h"

/**
 * @brief Construct a new CPU object
 * @param virtualMemoryUnit Memory Management Unit reference
 */
CPU::CPU(VirtualMemoryUnit& virtualMemoryUnit /**, BackingStoreManager& backingStore*/)
  : virtualMemoryUnit(virtualMemoryUnit)/**, backingStore(backingStore) */ {

}

/**
 * @brief Initialize backing store with test patterns
 */
void CPU::initialize_backingStore() {
  std::vector<char> page(PAGE_SIZE);
}

/**
 * @brief Execute a memory access sequence for demonstration
 * @param access_sequence Sequence of virtual page numbers to access
 */
void CPU::execute_access_sequence(const std::vector<size_t>& access_sequence) const {
  std::cout << "CPU: Starting memory access sequence...\n";

  for (const size_t vpn : access_sequence) {
    const char value = virtualMemoryUnit.read_memory(vpn, 0);
    std::cout << "CPU: Read vpn=" << std::setw(2) << vpn
              << " -> byte=" << static_cast<int>(static_cast<unsigned char>(value)) << "\n";

    // Small delay for readable output
    std::this_thread::sleep_for(std::chrono::milliseconds(30));
  }

  // Perform some writes
  this->virtualMemoryUnit.write_memory(4, 10, 0xAA);
  this->virtualMemoryUnit.write_memory(1, 20, 0xBB);

  // Access more pages
  this->virtualMemoryUnit.read_memory(9, 0);
  this->virtualMemoryUnit.read_memory(10, 0);
  this->virtualMemoryUnit.read_memory(4, 10);

  std::cout << "CPU: Memory access sequence completed.\n";
}

void CPU::shutdown() {
  this->virtualMemoryUnit.sync_dirty_pages();
  this->print_statistics();
}

void CPU::print_statistics() const {
  const auto& stats = virtualMemoryUnit.get_stats();

  std::cout << "\n--- Memory Access Statistics ---\n";
  std::cout << "Total accesses       : " << stats.accesses << "\n";
  std::cout << "Hits (resident)      : " << stats.hits << "\n";
  std::cout << "Page faults          : " << stats.page_faults << "\n";
  std::cout << "Swap-ins             : " << stats.swap_ins << "\n";
  std::cout << "Swap-outs            : " << stats.swap_outs << "\n";
  std::cout << "Page-table walks     : " << stats.page_table_walks
            << " (simulated levels: " << PAGE_TABLE_WALK_COST_LEVELS << " per access)\n";

  const double hit_ratio = (stats.accesses > 0) ?
      (static_cast<double>(stats.hits) / stats.accesses * 100.0) : 0.0;
  std::cout << "Effective hit ratio  : " << std::fixed << std::setprecision(2)
            << hit_ratio << "%\n";
}