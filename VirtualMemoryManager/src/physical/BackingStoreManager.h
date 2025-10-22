#ifndef BACKINGSTOREMANAGER_H
#define BACKINGSTOREMANAGER_H

#include <array>
#include <iostream>
#include <iomanip>
#include <stdexcept>

#include "../constans.h"

/**
 * @brief Simulates secondary storage (backing store / swap file)
 */
class BackingStoreManager {
private:
    /// Backing store: BACKING_PAGES × PAGE_SIZE bytes
    std::array<std::array<char, PAGE_SIZE>, BACKING_PAGES> backingStore{};

    /**
     * @brief Construct a new Backing Store Manager object
     */
    BackingStoreManager();

public:

    /**
    * @brief Returns the singleton instance of the BackingStoreManager.
    *
    * This method provides access to the single shared instance of the
    * BackingStoreManager class. It ensures that only one instance exists
    * throughout the system by creating it the first time it is called
    * and returning the same reference on subsequent calls.
    *
    * @return Reference to the singleton BackingStoreManager instance.
    */
    static BackingStoreManager& instance() {
      static BackingStoreManager instance;
      return instance;
    }

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

    /**
     * @brief Read a byte from a specific virtual page
     * @param vpn Virtual page number
     * @param offset Byte offset within the page
     * @return Byte value at the specified location
     */
    char read_byte(size_t vpn, size_t offset) const;

    /**
     * @brief Write a byte to a specific virtual page
     * @param vpn Virtual page number
     * @param offset Byte offset within the page
     * @param value Byte value to write
     */
    void write_byte(size_t vpn, size_t offset, char value);
};

#endif // BACKINGSTOREMANAGER_H
