#include <iostream>
#include "BackingStoreManager.h"

BackingStoreManager::BackingStoreManager() {
    // Initialize backing store with zeros
    for (auto& page : backingStore) {
        page.fill(0);
    }
}

void BackingStoreManager::load_page(size_t vpn, char* buffer) {
    if (vpn >= BACKING_PAGES) {
        throw std::out_of_range("Virtual page number out of range");
    }
    std::copy(backingStore[vpn].begin(), backingStore[vpn].end(), buffer);
}

void BackingStoreManager::save_page(size_t vpn, const char* buffer) {
    if (vpn >= BACKING_PAGES) {
        throw std::out_of_range("Virtual page number out of range");
    }
    std::copy(buffer, buffer + PAGE_SIZE, backingStore[vpn].begin());
}

void BackingStoreManager::initialize_test_patterns() {
    for (size_t vpn = 0; vpn < BACKING_PAGES; ++vpn) {
        for (size_t offset = 0; offset < PAGE_SIZE; ++offset) {
            backingStore[vpn][offset] = static_cast<char>((vpn + offset) % 256);
        }
    }
}

void BackingStoreManager::print_page_contents(size_t vpn) const {
    if (vpn >= BACKING_PAGES) {
        throw std::out_of_range("Virtual page number out of range");
    }
    std::cout << "Contents of virtual page " << vpn << ":\n";
    for (size_t offset = 0; offset < PAGE_SIZE; ++offset) {
        if (offset % 16 == 0) {
            std::cout << std::setw(4) << std::setfill('0') << std::hex << offset << ": ";
        }
        std::cout << std::setw(2) << std::setfill('0') << std::hex
                  << (static_cast<int>(backingStore[vpn][offset]) & 0xFF) << " ";
        if ((offset + 1) % 16 == 0) {
            std::cout << "\n";
        }
    }
    if (PAGE_SIZE % 16 != 0) {
        std::cout << "\n";
    }
}

char BackingStoreManager::read_byte(size_t vpn, size_t offset) const {
    if (vpn >= BACKING_PAGES || offset >= PAGE_SIZE) {
        throw std::out_of_range("Virtual page number or offset out of range");
    }
    return backingStore[vpn][offset];
}

void BackingStoreManager::write_byte(size_t vpn, size_t offset, char value) {
    if (vpn >= BACKING_PAGES || offset >= PAGE_SIZE) {
        throw std::out_of_range("Virtual page number or offset out of range");
    }
    backingStore[vpn][offset] = value;
}

