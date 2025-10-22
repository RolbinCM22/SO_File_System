#pragma once
#include <vector>
#include <cstdint>


struct pageTableEntry{
    int8_t frameNumber = 0;
    bool dirtyBit = false;
    bool present = false;
};