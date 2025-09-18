#include <iostream>
#include "FileSystem.hpp"
#include "Directory.hpp"

int main() {
    FileSystem fs;
    fs.loadDirectory();
    // Crear un archivo nuevo
    std::string filename = "test.txt";
    // std::string permisos = "rw";
    /*if (fs.createFile(filename, permisos) == 0) {
        std::cout << "Archivo creado correctamente." << std::endl;
    } else {
        std::cout << "Error al crear el archivo." << std::endl;
        return 1;
    }*/
   //fs.deleteFile(filename);
    fs.dir.printDirectory();
    // Escribir datos en el archivo
    /*std::string data = "Hola, este es un texto de prueba para el sistema de archivos. "
                       "Puedes agregar más texto para probar la asignación de varios bloques.";
    fs.writeFile(filename, data);*/
    
   /*FileSystem fs = FileSystem();
    fs.loadDirectory();
    fs.dir.printDirectory();
    fs.saveDirectory();
    fs.createFile("archivo1.txt", "rw-r--r--");*/
    /*Directory dir;
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
    }*/

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














