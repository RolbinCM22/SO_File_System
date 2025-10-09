cmake_minimum_required(VERSION 4.0)
project(VirtualMemoryManager)

set(CMAKE_CXX_STANDARD 20)

add_executable(VirtualMemoryManager main.cpp
        VirtualMemoryUnit.cpp
        VirtualMemoryUnit.h
        CPU.cpp
        CPU.h
        structures/MemoryStats.h
)
