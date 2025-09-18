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
  uint64_t directionDirectory; 
  void resetUnity();
  int loadBitMap();
  
public:
  FileSystem();
  ~FileSystem();
  Directory dir;
  int createFile(const std::string& filename, std::string permissions);
  int openFile(const std::string& filename);
  int closeFile(const std::string& filename);
  void writeFile(std::string filename, std::string& data);
  void readFile(uint64_t* location);
  void deleteFile(const std::string& filename);
  void listFiles();
  void searchFile(const std::string& filename);
  void saveInode(std::fstream& disk, const iNode& node, uint64_t offset);
  int initializeDirectory(); // <-- Función para inicializar el directorio
  int saveDirectory();
  int loadDirectory();
  iNode loadInode(std::fstream& disk, uint64_t offset);
};

