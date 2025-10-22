#ifndef BACKINGSTOREMANAGER_H
#define BACKINGSTOREMANAGER_H

#include <array>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "../VirtualMemoryUnit.h"

/**
 * @brief Simulates secondary storage (backing store / swap file)
 */
class BackingStoreManager {
private:
    /// Backing store: BACKING_PAGES × PAGE_SIZE bytes
    std::array<std::array<char, PAGE_SIZE>, BACKING_PAGES> backingStore{};

public:
    /**
     * @brief Construct a new Backing Store Manager object
     */
    BackingStoreManager();

    /**
     * @brief Load a page from backing store into a memory buffer
     * @param vpn Virtual page number
     * @param buffer Destination buffer (PAGE_SIZE bytes)
     */
    void load_page(size_t vpn, char* buffer);

    /**
     * @brief Save a page from memory buffer back to backing store
     * @param vpn Virtual page number
     * @param buffer Source buffer (PAGE_SIZE bytes)
     */
    void save_page(size_t vpn, const char* buffer);

    /**
     * @brief Initialize backing store with predictable data (for testing)
     */
    void initialize_test_patterns();

    /**
     * @brief Print contents of a specific virtual page (for debugging)
     * @param vpn Virtual page number
     */
    void print_page_contents(size_t vpn) const;
};

#endif // BACKINGSTOREMANAGER_H
