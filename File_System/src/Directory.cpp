#include "Directory.hpp"
#include <algorithm>
#include <cstdint>

Directory::Directory() {}
Directory::~Directory() {}

bool Directory::addToDirectory(const std::string& filename) {
    if (repeatName(filename)) return false;
    Entry entry;
    entry.filename = filename;
    entry.inodeNumber = static_cast<uint64_t>(files.size());
    files.push_back(entry);
    return true;
}

bool Directory::removeFromDirectory(const std::string& filename) {
    auto it = std::remove_if(files.begin(), files.end(), [&](const Entry& e) {
        return e.filename == filename;
    });
    if (it == files.end()) return false;
    files.erase(it, files.end());
    return true;
}

bool Directory::findInDirectory(const std::string& filename) {
    return std::any_of(files.begin(), files.end(), [&](const Entry& e) {
        return e.filename == filename;
    });
}

void Directory::listFiles() {
    for (const auto& entry : files) {
        std::cout << entry.filename << "\n";
    }
}

void Directory::printDirectory() {
    std::cout << "Directory contents:\n";
    listFiles();
}

bool Directory::repeatName(const std::string& filename) {
    return findInDirectory(filename);
}
