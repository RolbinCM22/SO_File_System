#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include "File.hpp"

class Directory {
 struct Entry {
  std::string filename;
  uint64_t inodeNumber;
 };
 private:
  std::vector<Entry> files;

 public:
  Directory();
  ~Directory();
  bool addToDirectory(const std::string& filename);
  bool removeFromDirectory(const std::string& filename);
  bool findInDirectory(const std::string& filename);
  void listFiles();
  void printDirectory();
  bool repeatName(const std::string& filename);
    
};