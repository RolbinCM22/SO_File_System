#pragma once
#include <iostream>
#include <string>
#include "iNode.hpp"

class File {
 private:
  iNode metadata;
 public:
  File();
  ~File();
  std::string getName() const;
  std::string getType() const;
  std::string getState() const;
  uint64_t getLength() const;
  iNode getMetadata() const;
  void open();
  void close();
};