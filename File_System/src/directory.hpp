#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "file.hpp"



struct directoryEntry
{
    std::string filename;
    uint64_t inodeNumber;
};

class directory {
    private:
        std::vector<directoryEntry> files;
    public:
        directory();
        ~directory();
        bool addToDirectory(const std::string& filename);
        bool removeFromDirectory(const std::string& filename);
        bool findInDirectory(const std::string& filename);    
        void listFiles();
        void printDirectory();
        bool repeatName(const std::string& filename);
    
};