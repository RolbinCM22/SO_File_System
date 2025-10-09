//
// Created by Aaroncz on 8/10/2025.
//

#ifndef VIRTUALMEMORYMANAGER_CPU_H
#define VIRTUALMEMORYMANAGER_CPU_H
#include <iostream>

#include "VirtualMemoryUnit.h"


/**
 * @brief Simulates a CPU that generates memory access requests
 */
class CPU {
private:
  VirtualMemoryUnit& virtualMemoryUnit;
  // BackingStoreManager& backingStore;

public:
  /**
   * @brief Construct a new CPU object
   * @param virtualMemoryUnit Memory Management Unit reference
   */
  explicit CPU(VirtualMemoryUnit& virtualMemoryUnit /**, BackingStoreManager& backingStore*/);

  /**
 * @brief Execute a memory access sequence for testing
 */
  void execute_access_sequence(const std::vector<size_t>& access_sequence) {
    std::cout << "=== CPU Test Sequence Started ===\n";

    // Test reads
    for (size_t vpn : access_sequence) {
      char value = virtualMemoryUnit.read_memory(vpn, 0);
      std::cout << "CPU: Read VPN=" << std::setw(2) << vpn
                << " -> Value=" << static_cast<int>(static_cast<unsigned char>(value))
                << "\n";
      std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Test writes
    std::cout << "\n--- Testing Writes ---\n";
    virtualMemoryUnit.write_memory(4, 10, 0xAA);
    std::cout << "CPU: Write to VPN=4, Offset=10, Value=0xAA\n";

    virtualMemoryUnit.write_memory(1, 20, 0xBB);
    std::cout << "CPU: Write to VPN=1, Offset=20, Value=0xBB\n";

    // Test more reads
    std::cout << "\n--- Testing Additional Reads ---\n";
    virtualMemoryUnit.read_memory(9, 0);
    std::cout << "CPU: Read VPN=9\n";

    virtualMemoryUnit.read_memory(10, 0);
    std::cout << "CPU: Read VPN=10\n";

    virtualMemoryUnit.read_memory(4, 10);
    std::cout << "CPU: Read VPN=4, Offset=10\n";

    std::cout << "=== CPU Test Sequence Completed ===\n\n";
  }

  /**
   * @brief Initialize backing store with test patterns
   */
  void initialize_backingStore();

  /**
   * @brief Execute a memory access sequence for demonstration
   * @param access_sequence Sequence of virtual page numbers to access
   */
  void execute_access_sequence(const std::vector<size_t>& access_sequence) const;

  /**
   * @brief Sync all dirty pages and print statistics
   */
  void shutdown();

private:
  /**
   * @brief Print memory access statistics
   */
  void print_statistics() const;
};


#endif //VIRTUALMEMORYMANAGER_CPU_H