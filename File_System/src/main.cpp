#include <iostream>
#include "FileSystem.hpp"
#include "Directory.hpp"

int main() {
    FileSystem fs;

    // Cargar el directorio inicial (si existe)
    fs.loadDirectory();
    std::cout << "Directorio inicial:" << std::endl;
    fs.listFiles();

    // Crear un archivo nuevo
    std::string filename = "test";
    std::string permisos = "rw";
    if (fs.createFile(filename, permisos) == 0) {
        std::cout << "Archivo '" << filename << "' creado correctamente." << std::endl;
    } else {
        std::cout << "Error al crear el archivo '" << filename << "'." << std::endl;
    }

    // Listar archivos después de crear
    std::cout << "\nDirectorio después de crear archivo:" << std::endl;
    fs.listFiles();

    // Buscar archivo
    std::cout << "\nBuscando archivo 'test.txt':" << std::endl;
    fs.searchFile(filename);

    // Mostrar el estado del bitmap
    std::cout << "\nEstado del bitmap (primeros 15 bloques):" << std::endl;
    fs.printBitMap();

    // Abrir el archivo
    if (fs.openFile(filename) == 0) {
        std::cout << "Archivo '" << filename << "' abierto correctamente." << std::endl;
    } else {
        std::cout << "Error al abrir el archivo '" << filename << "'." << std::endl;
    }

    // Escribir datos en el archivo
    std::string data = " Hola, este es un texto de prueba para el sistema de archivos. "
                       "Puedes agregar más texto para probar la asignación de varios bloques.";
    fs.writeFile(filename, data);

    // Leer datos del archivo
    std::cout << "\nLeyendo datos del archivo:" << std::endl;
    fs.readFile(filename);

    // Cerrar el archivo
    if (fs.closeFile(filename) == 0) {
        std::cout << "Archivo '" << filename << "' cerrado correctamente." << std::endl;
    } else {
        std::cout << "Error al cerrar el archivo '" << filename << "'." << std::endl;
    }
    // Crear y trabajar con "notes.txt"
    /*std::string filename2 = "notes.txt";
    fs.createFile(filename2, permisos);
    fs.openFile(filename2);
    std::string data2 = "These are some notes for the file system.";
    fs.writeFile(filename2, data2);
    fs.readFile(filename2);
    fs.closeFile(filename2);

    // Renombrar "notes.txt" a "notes_renamed.txt"
    fs.renameFile("notes.txt", "notes_renamed.txt");

    // Leer el archivo renombrado
    fs.openFile("notes_renamed.txt");
    fs.readFile("notes_renamed.txt");
    fs.closeFile("notes_renamed.txt");*/

    // Eliminar el archivo
    fs.deleteFile(filename);
    std::cout << "\nDirectorio después de eliminar archivo:" << std::endl;
    fs.listFiles();

    // Mostrar el estado del bitmap final
    std::cout << "\nEstado final del bitmap (primeros 15 bloques):" << std::endl;
    fs.printBitMap();

    return 0;
}














