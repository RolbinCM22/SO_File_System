#include <fstream>
#include "FileSystem.hpp"

FileSystem::FileSystem(){
  this->name = "MyFileSystem";
  this->block_size = 256; // 256 bits = 32 bytes

  initializeDisk();
}

FileSystem::~FileSystem(){

}

int FileSystem::initializeDisk(){
  std::fstream disk("./data/unity.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
    std::cerr << "Error initializing the disk" << std::endl;
    return -1;
  }

 

  disk.close();
  loadBitMap();
  std::cout << "Disk initialized successfully and magic number write." << std::endl;
  return 0;
}


int FileSystem::loadBitMap() {
  
  return 0;
}

int FileSystem::saveDirectory() {
  std::fstream disk("./data/unity.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
      std::cerr << "Error initializing the disk" << std::endl;
  }
  // Guardar la estructura de directorio en el disco
  disk.close();
  return -1;
}

void FileSystem::createFile(const std::string& filename, uint64_t size) {
  
}

int FileSystem::openFile(const std::string& filename) {
  for(auto& file : files) {
    if(file.name == filename) {
      file.state = "open";
      return 0;
    }
  }
  std::cerr << "El archivo no existe." << std::endl;
  return -1;
}

int FileSystem::closeFile(const std::string& filename) {
  for(auto& file : files) {
    if(file.name == filename) {
      file.state = "closed";
      return 0;
    }
  }
  std::cerr << "El archivo no existe." << std::endl;
  return -1;
}

int FileSystem::writeFile(const std::string& filename, std::string& data) {
  
}

void FileSystem::readFile(uint64_t* location) {

}
void deleteFile(const std::string& filename);
void searchFile(const std::string& filename);

