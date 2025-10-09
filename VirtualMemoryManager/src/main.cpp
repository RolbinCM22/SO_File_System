#include <iostream>
#include <memory>
#include <vector>

#include "CPU.h"
#include "structures/MemoryStats.h"


/**
 * @brief Test function to verify CPU-VMU connection
 */
void test_cpu_vmu_connection() {
  std::cout << "🔧 Testing CPU - VirtualMemoryUnit Connection\n";
  std::cout << "=============================================\n\n";

  // Create components
  MemoryStats stats;
  VirtualMemoryUnit vmu(stats);
  CPU cpu(vmu);

  // Test sequence - designed to test various scenarios
  std::vector<size_t> test_sequence = {
    0, 1, 2, 3,     // Sequential access
    0, 1,           // Re-access (should be hits)
    4, 5,           // New pages
    2,              // Re-access
    6, 7, 8,        // More new pages
    1               // Final re-access
};

  std::cout << "Test sequence: ";
  for (size_t vpn : test_sequence) {
    std::cout << vpn << " ";
  }
  std::cout << "\n\n";

  // Execute test
  cpu.execute_access_sequence(test_sequence);
  cpu.shutdown();
}

/**
 * @brief Test error handling
 */
void test_error_handling() {
  std::cout << "\n🔧 Testing Error Handling\n";
  std::cout << "=========================\n";

  MemoryStats stats;
  VirtualMemoryUnit vmu(stats);

  try {
    // Test invalid VPN
    vmu.read_memory(BACKING_PAGES + 10, 0);
    std::cout << "❌ ERROR: Should have thrown exception for invalid VPN\n";
  } catch (const std::out_of_range& e) {
    std::cout << "✅ Correctly caught invalid VPN: " << e.what() << "\n";
  }

  try {
    // Test invalid offset
    vmu.read_memory(0, PAGE_SIZE + 100);
    std::cout << "❌ ERROR: Should have thrown exception for invalid offset\n";
  } catch (const std::out_of_range& e) {
    std::cout << "✅ Correctly caught invalid offset: " << e.what() << "\n";
  }

  try {
    // Test valid access
    vmu.read_memory(5, 100);
    std::cout << "✅ Valid access completed successfully\n";
  } catch (const std::exception& e) {
    std::cout << "❌ ERROR: Valid access failed: " << e.what() << "\n";
  }
}

int main() {
  std::cout << "🚀 CPU - VirtualMemoryUnit Integration Test\n";
  std::cout << "===========================================\n\n";

  try {
    // Test normal operation
    test_cpu_vmu_connection();

    // Test error handling
    test_error_handling();

    std::cout << "\n🎉 All tests completed successfully!\n";

  } catch (const std::exception& e) {
    std::cerr << "\n💥 Test failed with error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
