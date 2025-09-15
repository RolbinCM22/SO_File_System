#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <ctime>

struct iNode {
  std::string name;
  std::string type;
  std::string state;
  uint64_t length;
  uint64_t inodeId;
  std::string fileType;
  uint64_t fileSize;
  std::time_t creationTime;
  std::time_t modificationTime;
  std::vector<uint64_t> BlockPointers;
  std::string permissions;
};
