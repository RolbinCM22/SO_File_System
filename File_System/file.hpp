#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "inode.hpp"

class file {
    private:
        inode metadata;
    public:
        file();
        ~file();
        std::string getName() const;
        std::string getType() const;
        std::string getState() const;
        uint64_t getLength() const;
        iNode getMetadata() const;
        void open();
        void close();
};