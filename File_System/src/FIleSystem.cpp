#include <fstream>
#include "FileSystem.hpp"

FileSystem::FileSystem(){
  this->name = "MyFileSystem";
  this->block_size = 256; // 256 bits = 32 bytes
  this->size = 1024 * 1024 * 1024; // 1 GB

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

  disk.seekp(0, std::ios::beg);
  disk.read(reinterpret_cast<char*>(&this->directionDirectory), sizeof(this->directionDirectory));
  std::cout << "Directory initialized at block: " << this->directionDirectory << std::endl;
  disk.close();

  loadDirectory();
  loadBitMap();
 
  std::cout << "Disk initialized successfully." << std::endl;
  return 0;
}


int FileSystem::loadBitMap() {
  std::fstream disk("./data/unity.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
    std::cerr << "Error initializing the disk" << std::endl;
    return -1;
  }
  disk.seekg(0, std::ios::end);
  this->blocks = this->size / this->block_size;
  this->bitMap.resize(this->blocks, false);
  for(size_t i = 0; i < this->blocks; ++i) {
    char byte;
    disk.seekg(i * this->block_size, std::ios::beg);
    disk.read(&byte, sizeof(byte));
    if(byte != 0) {
      this->bitMap[i] = true; // Bloque ocupado
    }
  }
  for(size_t i = 0; i < 5; ++i) {
    std::cout << "Block " << i << ": " << (this->bitMap[i] ? "Occupied" : "Free") << std::endl;
  }
  disk.close();
  
  return 0;
}
int FileSystem::createFile(const std::string& filename, std::string permissions) {
  if(this->dir.repeatName(filename)) {
    std::cerr << "El archivo ya existe." << std::endl;
    return -1;
  }

  iNode newFile;
  newFile.name = filename;
  newFile.state = "closed";
  newFile.type = "txt";
  newFile.permissions = permissions;
  uint64_t freeblock = 0;
  for(size_t i = 3; i < this->bitMap.size(); ++i) {
    if(!this->bitMap[i]) {
      this->bitMap[i] = true;
      freeblock = i;
      break;
    }
  }
  newFile.id = freeblock;
  this->dir.addToDirectory(filename, newFile.id);

  std::fstream disk("./data/unity.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
    std::cerr << "Error initializing the disk" << std::endl;
    return -1;
  }
  saveInode(disk, newFile, freeblock * this->block_size);
  
  dir.printDirectory();  
  return 0;
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


void FileSystem::saveInode(std::fstream& disk, const iNode& node, uint64_t offset) {
  disk.seekp(offset, std::ios::beg);

  uint64_t nameLen = node.name.size();
  disk.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
  disk.write(node.name.c_str(), nameLen);

  
  uint64_t typeLen = node.type.size();
  disk.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
  disk.write(node.type.c_str(), typeLen);

  
  uint64_t stateLen = node.state.size();
  disk.write(reinterpret_cast<const char*>(&stateLen), sizeof(stateLen));
  disk.write(node.state.c_str(), stateLen);

  
  disk.write(reinterpret_cast<const char*>(&node.id), sizeof(node.id));
  uint64_t permissionsLen = node.permissions.size();
  disk.write(reinterpret_cast<const char*>(&permissionsLen), sizeof(permissionsLen));
  disk.write(node.permissions.c_str(), permissionsLen);
  
  
  uint64_t blocksCount = node.BlockPointers.size();
  disk.write(reinterpret_cast<const char*>(&blocksCount), sizeof(blocksCount));
  for (auto block : node.BlockPointers) {
      disk.write(reinterpret_cast<const char*>(&block), sizeof(block));
  }
}

int FileSystem::saveDirectory() {
  std::fstream disk("./data/unity.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
      std::cerr << "Error initializing the disk" << std::endl;
  }

  disk.seekp(this->directionDirectory * this->block_size, std::ios::beg);
  uint64_t filesCount = this->dir.files.size();
  disk.write(reinterpret_cast<const char*>(&filesCount), sizeof(filesCount));
  for (const auto& entry : this->dir.files) {
      uint64_t nameLen = entry.filename.size();
      disk.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
      disk.write(entry.filename.c_str(), nameLen);
      disk.write(reinterpret_cast<const char*>(&entry.inodeNumber), sizeof(entry.inodeNumber));
  }
  std::cout << "Directory saved successfully." << std::endl;
  disk.close();
  return 0;
}
int FileSystem::loadDirectory() {
  std::fstream disk("./data/unity.bin", std::ios::in | std::ios::binary);
  if (!disk) {
      std::cerr << "Error initializing the disk" << std::endl;
      return -1;
  }

  disk.seekg(this->directionDirectory * this->block_size, std::ios::beg);
  uint64_t filesCount = 0;
  disk.read(reinterpret_cast<char*>(&filesCount), sizeof(filesCount));
  this->dir.files.clear();

  for (uint64_t i = 0; i < filesCount; ++i) {
    Directory::Entry entry;
    uint64_t nameLen = 0;
    disk.read(reinterpret_cast<char*>(&nameLen), sizeof(nameLen));
    entry.filename.resize(nameLen);
    disk.read(&entry.filename[0], nameLen);
    disk.read(reinterpret_cast<char*>(&entry.inodeNumber), sizeof(entry.inodeNumber));
    this->dir.files.push_back(entry);
  }

  disk.close();
  std::cout << "Directory loaded successfully." << std::endl;
  return 0;
}

void FileSystem::resetUnity() {
  std::ofstream disk("./data/unity.bin", std::ios::out | std::ios::binary | std::ios::trunc);
  if (!disk) {
    std::cerr << "Error initializing the disk" << std::endl;
    return;
  }
  char zero = 0;
  for(size_t i = 0; i < this->blocks; ++i) {
    disk.write(&zero, sizeof(zero));
  }
  disk.close();
}

