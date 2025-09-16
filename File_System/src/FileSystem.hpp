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
    uint64_t size;
    uint64_t free_size;
    uint64_t block_size;
    uint64_t blocks;
    std::string identified;
    std::string type;
    std::vector<bool> bitMap;
    std::vector<iNode> files;
    //directory direc; 
    int initializeDirectory(); // <-- Función para inicializar el directorio
    int loadBitMap();
    int saveDirectory();
public:
    FileSystem();
    ~FileSystem();
    void createFile(const std::string& filename, uint64_t size);
    int openFile(const std::string& filename);
    int closeFile(const std::string& filename);
    void writeFile(std::string identified, std::string& data);
    void readFile(uint64_t* location);
    void deleteFile(const std::string& filename);
    void searchFile(const std::string& filename);
    
};

