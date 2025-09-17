#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdint>
#include "File.hpp"

class Directory {
 struct Entry {
  std::string filename;
  uint64_t inodeNumber;
 };
 private:
  

 public:
  std::vector<Entry> entries;
  Directory();
  ~Directory();
  bool addToDirectory(const std::string& filename, uint64_t inodeNumber);
  bool removeFromDirectory(const std::string& filename);
  bool findInDirectory(const std::string& filename);
  void listFiles();
  void printDirectory();
  bool repeatName(const std::string& filename);
    
};