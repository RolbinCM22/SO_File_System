//
// Created by Aaroncz on 21/10/2025.
//

#ifndef VIRTUALMEMORYMANAGER_CONSTANS_H
#define VIRTUALMEMORYMANAGER_CONSTANS_H

#include <iomanip>

// Configuration constants
static constexpr size_t PAGE_SIZE = 4096;           ///< Page size in bytes
static constexpr size_t NUM_FRAMES = 4;             ///< Physical frames
static constexpr size_t BACKING_PAGES = 64;         ///< Virtual pages
static constexpr size_t PAGE_TABLE_WALK_COST_LEVELS = 2;  ///< Necessary indirections to reach page on logic memory.
static constexpr size_t FRAME_SIZE = 256;  ///< Necessary indirections to reach page on logic memory.

#endif //VIRTUALMEMORYMANAGER_CONSTANS_H