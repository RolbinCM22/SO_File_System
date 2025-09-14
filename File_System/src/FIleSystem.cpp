#ifndef FILESYSTEM_H
#define FILESYSTEM_H
#include <fstream>
#include "FileSystem.hpp"


FileSystem::FileSystem(){
    this->name = "MyFileSystem";
    this->total_size = 2 * 1024 * 1024; // 2 MB
    this->block_size = total_size / 256; // 8 KB
    this->blocks = total_size / block_size;
    this->free_size = total_size;
    
    std::ofstream disk("disk.bin", std::ios::binary);
    if (!disk){
        std::cerr << "Error accessing the disk" << std::endl;
    }
}




#endif 