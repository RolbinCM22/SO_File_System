#include <fstream>
#include "FileSystem.hpp"

FileSystem::FileSystem(){
  this->name = "MyFileSystem";
  this->block_size = 256; // 256 bits = 32 bytes

  initializeDirectory();
}

FileSystem::~FileSystem(){

}

int FileSystem::initializeDirectory(){
  std::fstream disk("./data/unity.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
    std::cerr << "Error initializing the disk" << std::endl;
    return -1;
  }
  uint64_t directionDirectory; // El directorio comienza en el bloque 2
  disk.seekp(this->block_size, std::ios::beg);
  disk.read(reinterpret_cast<char*>(&directionDirectory), sizeof(directionDirectory));
  std::cout << "Directory initialized at block: " << directionDirectory << std::endl;
  disk.close();
  loadBitMap();
  std::cout << "Disk initialized successfully and magic number write." << std::endl;
  return 0;
}


int FileSystem::loadBitMap() {
  std::fstream disk("./data/unity.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
    std::cerr << "Error initializing the disk" << std::endl;
    return -1;
  }
  this->blocks = disk.tellg() / this->block_size;
  this->bitMap.resize(this->blocks, false);
  for(int i = 0; i < this->blocks; ++i) {
    char byte;
    disk.seekg(i * this->block_size, std::ios::beg);
    disk.read(&byte, sizeof(byte));
    if(byte != 0) {
      this->bitMap[i] = true; // Bloque ocupado
    }
  }
  disk.close();
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

//void FileSystem::createFile(const std::string& filename, uint64_t size);

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

//int FileSystem::writeFile(const std::string& filename, std::string& data);

//void FileSystem::readFile(uint64_t* location);
void deleteFile(const std::string& filename);
void searchFile(const std::string& filename);

