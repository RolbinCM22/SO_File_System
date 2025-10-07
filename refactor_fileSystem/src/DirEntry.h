#pragma once
#include <cstdint>
#include "Layout.h"


struct DirEntry {
    char     name[Layout::DIR_NAME_LEN]; 
    uint64_t inode_id;                   
};

