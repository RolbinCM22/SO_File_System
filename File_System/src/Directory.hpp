/**
 * @file Directory.hpp
 * @brief Declaración de la clase Directory para la gestión de archivos y subdirectorios.
 */
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <cstdint>
#include "File.hpp"

/**
 * @class Directory
 * @brief Representa un directorio que almacena referencias a archivos mediante nombre e iNode.
 */
class Directory {
    /**
     * @struct Entry
     * @brief Estructura que representa una entrada en el directorio (archivo o subdirectorio).
     */
    struct Entry {
        std::string filename; /**< Nombre del archivo */
        uint64_t inodeNumber; /**< Número de iNode asociado al archivo */
    };
private:
    std::vector<Entry> files; /**< Vector de entradas del directorio */

public:
    /**
     * @brief Constructor por defecto del directorio.
     */
    Directory();
    /**
     * @brief Destructor del directorio.
     */
    ~Directory();
    /**
     * @brief Agrega un archivo al directorio.
     * @param filename Nombre del archivo a agregar.
     * @return true si se agregó correctamente, false si el nombre ya existe.
     */
    bool addToDirectory(const std::string& filename);
    /**
     * @brief Elimina un archivo del directorio.
     * @param filename Nombre del archivo a eliminar.
     * @return true si se eliminó correctamente, false si no se encontró.
     */
    bool removeFromDirectory(const std::string& filename);
    /**
     * @brief Busca un archivo en el directorio.
     * @param filename Nombre del archivo a buscar.
     * @return true si se encontró, false en caso contrario.
     */
    bool findInDirectory(const std::string& filename);
    /**
     * @brief Lista los archivos contenidos en el directorio.
     */
    void listFiles();
    /**
     * @brief Imprime el contenido del directorio en consola.
     */
    void printDirectory();
    /**
     * @brief Verifica si el nombre de archivo ya existe en el directorio.
     * @param filename Nombre a verificar.
     * @return true si el nombre está repetido, false en caso contrario.
     */
    bool repeatName(const std::string& filename);
    
};