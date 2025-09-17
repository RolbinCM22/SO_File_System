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
  int freeblock = -1;
  for(int i = 4; i < this->bitMap.size(); ++i) {
    if(!this->bitMap[i]) {
      freeblock = i;
      break;
    }
  }
  if(freeblock == -1) {
    freeblock = this->bitMap.size();
    this->bitMap.push_back(true);
    newFile.BlockPointers.push_back(freeblock);
    return;
  }
  newFile.id = freeblock;
  fstream disk("./data/unity.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
    std::cerr << "Error initializing the disk" << std::endl;
    return;
  }

  this->dir.addToDirectory(filename, newFile.id);


  saveInode(disk, newFile, freeblock * this->block_size);
  disk.close();
  this->files.push_back(newFile);
  
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

void FileSystem::saveInode(std::fstream& disk, const iNode& node, uint64_t offset) {
    disk.seekp(offset, std::ios::beg);

    // Serializar y guardar el nombre
    uint64_t nameLen = node.name.size();
    disk.write(reinterpret_cast<const char*>(&nameLen), sizeof(nameLen));
    disk.write(node.name.c_str(), nameLen);

    // Serializar y guardar el tipo
    uint64_t typeLen = node.type.size();
    disk.write(reinterpret_cast<const char*>(&typeLen), sizeof(typeLen));
    disk.write(node.type.c_str(), typeLen);

    // Serializar y guardar el estado
    uint64_t stateLen = node.state.size();
    disk.write(reinterpret_cast<const char*>(&stateLen), sizeof(stateLen));
    disk.write(node.state.c_str(), stateLen);

    // Guardar el id
    disk.write(reinterpret_cast<const char*>(&node.id), sizeof(node.id));
    uint64_t permissionsLen = node.permissions.size();
    disk.write(reinterpret_cast<const char*>(&permissionsLen), sizeof(permissionsLen));
    disk.write(node.permissions.c_str(), permissionsLen);
    
    // Serializar y guardar los bloques asignados
    uint64_t blocksCount = node.BlockPointers.size();
    disk.write(reinterpret_cast<const char*>(&blocksCount), sizeof(blocksCount));
    for (auto block : node.BlockPointers) {
        disk.write(reinterpret_cast<const char*>(&block), sizeof(block));
    }
}

