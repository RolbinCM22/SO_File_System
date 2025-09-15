#include <fstream>
#include "FileSystem.hpp"

FileSystem::FileSystem(){
  this->name = "MyFileSystem";
  this->total_size = 2 * 1024 * 1024; // 2 MB
  this->block_size = total_size / 256; // 8 KB
  this->blocks = total_size / block_size;
  this->free_size = total_size;
  initializeDisk();
}

FileSystem::~FileSystem(){

}

int FileSystem::initializeDisk(){
  std::fstream disk("./data/disk.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
    std::cerr << "Error initializing the disk" << std::endl;
    return -1;
  }
  
  uint16_t magicNumber = 0xAA55;
  
  disk.seekp(block_size - sizeof(magicNumber), std::ios::beg);
  disk.read(reinterpret_cast<char*>(&magicNumber), sizeof(magicNumber));
  if( magicNumber != 0xAA55) {
    std::cerr << "Archivo de sistema de archivos inválido." << std::endl;
    return -1;
  }

  disk.close();
  std::cout << "Disk initialized successfully and magic number write." << std::endl;
  return 0;
}



int createFile(const std::string& filename, uint64_t size){
    
    
    return 0;
  }
 std::string openFile(const std::string& filename);
    void writeFile(std::string identified, std::string& data);
    void readFile(uint64_t* location);
    void deleteFile(const std::string& filename);
    void searchFile(const std::string& filename);   