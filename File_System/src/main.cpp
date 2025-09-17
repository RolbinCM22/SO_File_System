#include <iostream>
#include "FileSystem.hpp"
#include "Directory.hpp"

int main() {

    FileSystem fs = FileSystem();
    fs.loadDirectory();
    fs.dir.printDirectory();
    fs.saveDirectory();
    
    Directory dir;
    dir.addToDirectory("test1.txt", 101);
    dir.addToDirectory("test2.txt", 202);
    dir.addToDirectory("test3.txt", 303);
    dir.printDirectory();
    std::string buscar = "test2.txt";
    uint64_t inode = dir.findInDirectory(buscar);
    if (inode != UINT64_MAX) {
        std::cout << "El archivo '" << buscar << "' tiene iNode: " << inode << std::endl;
    } else {
        std::cout << "El archivo '" << buscar << "' no se encontró en el directorio." << std::endl;
    }

    /*std::cout << "Agregando archivo 'test1.txt': " << (dir.addToDirectory("test1.txt") ? "OK" : "FALLÓ") << std::endl;
    std::cout << "Agregando archivo 'test2.txt': " << (dir.addToDirectory("test2.txt") ? "OK" : "FALLÓ") << std::endl;
    std::cout << "Agregando archivo repetido 'test1.txt': " << (dir.addToDirectory("test1.txt") ? "OK" : "FALLÓ") << std::endl;
    std::cout << "Buscando 'test2.txt': " << (dir.findInDirectory("test2.txt") ? "ENCONTRADO" : "NO ENCONTRADO") << std::endl;
    std::cout << "Eliminando 'test1.txt': " << (dir.removeFromDirectory("test1.txt") ? "OK" : "NO ENCONTRADO") << std::endl;
    std::cout << "Eliminando 'test3.txt' (no existe): " << (dir.removeFromDirectory("test3.txt") ? "OK" : "NO ENCONTRADO") << std::endl;
    */

    // Return Statement*/
    return 0;
}














