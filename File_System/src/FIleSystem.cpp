#include <fstream>
#include "FileSystem.hpp"

FileSystem::FileSystem(){
  this->name = "MyFileSystem";
  this->total_size = 2 * 1024 * 1024; // 2 MB
  this->block_size = total_size / 256; // 8 KB
  this->blocks = total_size / block_size;
  this->free_size = total_size;
  this->magicNumber = 0xAA55;
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

  disk.seekp(block_size - sizeof(this->magicNumber), std::ios::beg);
  disk.read(reinterpret_cast<char*>(&this->magicNumber), sizeof(this->magicNumber));
  
  if(this->magicNumber != 0xAA55) {
    std::cerr << "Archivo de sistema de archivos inválido." << std::endl;
    return -1;
  }

  disk.close();
  loadBitMap();
  std::cout << "Disk initialized successfully and magic number write." << std::endl;
  return 0;
}


int FileSystem::loadBitMap() {
  std::fstream disk("./data/disk.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
      std::cerr << "Error initializing the disk" << std::endl;
      return -1;
  }
  uint64_t number;
  for (int i = 3; i < this->blocks; ++i) {
    disk.seekp(block_size * i, std::ios::beg);
    disk.read(reinterpret_cast<char*>(&this->number), sizeof(this->number));
    if(this->number == 1) {
      this->bitMap[i] = true;
    }
  }
  disk.close();
  return 0;
}

int FileSystem::saveDirectory() {
  std::fstream disk("./data/disk.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk) {
      std::cerr << "Error initializing the disk" << std::endl;
  }
  // Guardar la estructura de directorio en el disco
  disk.close();
  return -1;
}

void FileSystem::createFile(const std::string& filename, uint64_t size) {
  std::fstream disk("./data/disk.bin", std::ios::in | std::ios::out | std::ios::binary);
  if (!disk){
    std::cerr << "Error initializing the disk" << std::endl;
    return;
  }
  if(size > this->free_size) {
    std::cerr << "No hay espacio suficiente en el sistema de archivos." << std::endl;
    return;
  }
  for(const auto& file : files) {
    if(file.name == filename) {
      std::cerr << "El archivo ya existe." << std::endl;
      return;
    }
  }
  
  File newFile;
  newFile.metadata.name = filename;
  newFile.metadata.type = "txt";
  newFile.metadata.state = "closed";
  newFile.metadata.length = size;
  newFile.metadata.inodeId = files.size() + 1;
  files.push_back(newFile.metadata);
  free_size -= size;
  uint64_t blocksNeeded = (size + block_size - 1) / block_size;
  
  int contador = 4;
  
  while(blocksNeeded > 0) {
    if(contador >= blocks) {
      std::cerr << "No hay bloques libres disponibles." << std::endl;
      return;
    }
    if(!bitMap[contador]) {
      bitMap[contador] = true;
      newFile.metadata.BlockPointers.push_back(contador);
      blocksNeeded--;
    }
    contador++;
  }

  if (contador == blocks) {
    std::cerr << "No hay bloques libres disponibles." << std::endl;
    return;
  }
  
  disk.seekp(block_size * contador, std::ios::beg);
  disk.write(reinterpret_cast<const char*>(&newFile), sizeof(newFile));
  disk.close();
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
  for(auto& file : files) {
    if(file.name == filename){
      if(file.state != "open") {
        std::cerr << "El archivo no está abierto." << std::endl;
        return -1;
      }
      file.content = data;
      file.metadata.length = file.content.size() + calculateNodeSize(file.metadata);
      uint64_t blockneeded = (file.metadata.length + block_size - 1) / block_size;
      if(blockneeded > this-> free_size / block_size) {
        std::cerr << "No hay espacio suficiente en el sistema de archivos." << std::endl;
        return -1;
      }
      int64_t contador = 4; // Empezar a buscar desde el bloque 4

      while(blockneeded > 0) {
        if(contador >= blocks) {
          std::cerr << "No hay bloques libres disponibles." << std::endl;
          return;
        }
        if(!bitMap[contador]) {
          bitMap[contador] = true;
          file.metadata.BlockPointers.push_back(contador);
          blockneeded--;
        }
        contador++;
      }
    }
}

void FileSystem::readFile(uint64_t* location) {

}
  void deleteFile(const std::string& filename);
  void searchFile(const std::string& filename);

size_t calculateNodeSize(const iNode& node) {
    size_t total = 0;

    // Campos fijos
    total += sizeof(node.inodeId);
    total += sizeof(node.length);
    total += sizeof(node.modificationTime);

    // Strings: suma el tamaño del string + tamaño para guardar la longitud
    total += sizeof(uint64_t) + node.name.size();
    total += sizeof(uint64_t) + node.type.size();
    total += sizeof(uint64_t) + node.state.size();

    // Vector de bloques: tamaño para la cantidad + los bloques
    total += sizeof(uint64_t); // cantidad de bloques
    total += node.BlockPointers.size() * sizeof(uint64_t);

    return total;
}