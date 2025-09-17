/**
 * @file Directory.cpp
 * @brief Implementación de la clase Directory para la gestión de archivos y subdirectorios.
 */
#include "Directory.hpp"
#include <algorithm>
#include <cstdint>

/**
 * @brief Constructor por defecto del directorio.
 */
Directory::Directory() {}

/**
 * @brief Destructor del directorio.
 */
Directory::~Directory() {}

/**
 * @brief Agrega un archivo al directorio si el nombre no está repetido.
 * @param filename Nombre del archivo a agregar.
 * @return true si se agregó correctamente, false si el nombre ya existe.
 */
bool Directory::addToDirectory(const std::string& filename) {
    if (repeatName(filename)) return false;
    Entry entry;
    entry.filename = filename;
    entry.inodeNumber = static_cast<uint64_t>(files.size());
    files.push_back(entry);
    return true;
}

/**
 * @brief Elimina un archivo del directorio por nombre.
 * @param filename Nombre del archivo a eliminar.
 * @return true si se eliminó correctamente, false si no se encontró.
 */
bool Directory::removeFromDirectory(const std::string& filename) {
    auto it = std::remove_if(files.begin(), files.end(), [&](const Entry& e) {
        return e.filename == filename;
    });
    if (it == files.end()) return false;
    files.erase(it, files.end());
    return true;
}

/**
 * @brief Busca un archivo en el directorio por nombre.
 * @param filename Nombre del archivo a buscar.
 * @return true si se encontró, false en caso contrario.
 */
bool Directory::findInDirectory(const std::string& filename) {
    return std::any_of(files.begin(), files.end(), [&](const Entry& e) {
        return e.filename == filename;
    });
}

/**
 * @brief Lista los archivos contenidos en el directorio.
 */
void Directory::listFiles() {
    for (const auto& entry : files) {
        std::cout << entry.filename << "\n";
    }
}

/**
 * @brief Imprime el contenido del directorio en consola.
 */
void Directory::printDirectory() {
    std::cout << "Directory contents:\n";
    listFiles();
}

/**
 * @brief Verifica si el nombre de archivo ya existe en el directorio.
 * @param filename Nombre a verificar.
 * @return true si el nombre está repetido, false en caso contrario.
 */
bool Directory::repeatName(const std::string& filename) {
    return findInDirectory(filename);
}
