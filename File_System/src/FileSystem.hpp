#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include <ctime>
#include "iNode.hpp"
#include "Directory.hpp"

class FileSystem {
    private:
        std::string name; 
        uint64_t total_size;
        uint64_t free_size;
        uint64_t block_size;
        uint64_t blocks;
        std::string identified;
        std::string type;
        std::vector<bool> bitMap;
        std::vector<iNode> files;
        //directory direc; 
        int initializeDisk(); // <-- Función para inicializar el disco
    public:
        FileSystem();
        ~FileSystem();
        void createFile(const std::string& filename, uint64_t size);
        std::string openFile(const std::string& filename);
        void writeFile(std::string identified, std::string& data);
        void readFile(uint64_t* location);
        void deleteFile(const std::string& filename);
        void searchFile(const std::string& filename);
    


};

