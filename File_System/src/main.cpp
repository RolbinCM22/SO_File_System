#include <iostream>
#include "FileSystem.hpp"
#include "Directory.hpp"

int main() {

    FileSystem fs = FileSystem();
    Directory dir;

    std::cout << "Agregando archivo 'test1.txt': " << (dir.addToDirectory("test1.txt") ? "OK" : "FALLÓ") << std::endl;
    std::cout << "Agregando archivo 'test2.txt': " << (dir.addToDirectory("test2.txt") ? "OK" : "FALLÓ") << std::endl;
    std::cout << "Agregando archivo repetido 'test1.txt': " << (dir.addToDirectory("test1.txt") ? "OK" : "FALLÓ") << std::endl;
    std::cout << "Buscando 'test2.txt': " << (dir.findInDirectory("test2.txt") ? "ENCONTRADO" : "NO ENCONTRADO") << std::endl;
    std::cout << "Eliminando 'test1.txt': " << (dir.removeFromDirectory("test1.txt") ? "OK" : "NO ENCONTRADO") << std::endl;
    std::cout << "Eliminando 'test3.txt' (no existe): " << (dir.removeFromDirectory("test3.txt") ? "OK" : "NO ENCONTRADO") << std::endl;
    dir.printDirectory();

    // Return Statement
    return 0;
}














