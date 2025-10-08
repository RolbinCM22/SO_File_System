#include "Layout.h"
#include <iostream>
#include "FileSystem.hpp"
#include <chrono>
#include <random>
#include <unordered_map>
#include <vector>
#include <string>
#include <cstdlib>

// Función auxiliar para mostrar separadores
void showSeparator(const std::string& testName) {
    std::cout << "\n========================================\n";
    std::cout << "   " << testName << "\n";
    std::cout << "========================================\n";
}

// Función auxiliar para verificar resultados
void checkResult(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << testName << " - EXITOSO\n";
    } else {
        std::cout  << testName << " - FALLÓ\n";
    }
}

int main() {
    const std::string diskPath = "unity.bin";
    
    std::cout << "🚀 INICIANDO PRUEBAS DEL FILESYSTEM\n";
    std::cout << "Archivo de disco: " << diskPath << "\n";

    try {
        FileSystem fs(diskPath);
        
        // Si es necesario, descomenta la siguiente línea para formatear
        // fs.format();

     /*  showSeparator("1. PRUEBA DE CREACIÓN DE ARCHIVOS");
        
        // Test 1: Crear archivos
        int id1 = fs.create("documento.txt");
        checkResult(id1 >= 0, "Crear 'documento.txt'");
        
        int id2 = fs.create("imagen.dat");
        checkResult(id2 >= 0, "Crear 'imagen.dat'");
        
        int id3 = fs.create("config.json");
        checkResult(id3 >= 0, "Crear 'config.json'");

        // Test: Intentar crear archivo duplicado
        int idDup = fs.create("documento.txt");
        checkResult(idDup < 0, "Rechazar archivo duplicado");

        showSeparator("2. PRUEBA DE ESCRITURA");
        
        // Test 2: Escribir contenido
        std::string contenido1 = "Este es el contenido del documento principal.\n"
                                "Contiene varias líneas de texto.\n"
                                "¡Y caracteres especiales! @#$%^&*()";
        
        bool writeOk1 = fs.write("documento.txt", contenido1);
        checkResult(writeOk1, "Escribir contenido en 'documento.txt'");

        std::string contenido2 = "Datos binarios simulados: \x01\x02\x03\x04\xFF\xFE";
        bool writeOk2 = fs.write("imagen.dat", contenido2);
        checkResult(writeOk2, "Escribir datos binarios en 'imagen.dat'");

        // Archivo JSON de configuración
        std::string jsonConfig = "{\n"
                                "  \"version\": \"1.0\",\n"
                                "  \"debug\": true,\n"
                                "  \"max_files\": 1000\n"
                                "}";
        bool writeOk3 = fs.write("config.json", jsonConfig);
        checkResult(writeOk3, "Escribir JSON en 'config.json'");

        showSeparator("3. PRUEBA DE LECTURA");
        
        // Test 3: Leer contenido
        std::string readContent1 = fs.read("documento.txt");
        checkResult(readContent1 == contenido1, "Leer 'documento.txt' correctamente");
        
        std::string readContent2 = fs.read("imagen.dat");
        checkResult(readContent2 == contenido2, "Leer 'imagen.dat' correctamente");
        
        std::string readContent3 = fs.read("config.json");
        checkResult(readContent3 == jsonConfig, "Leer 'config.json' correctamente");

        // Test: Leer archivo inexistente
        std::string noExiste = fs.read("noexiste.txt");
        checkResult(noExiste.empty(), "Archivo inexistente retorna vacío");

        showSeparator("4. PRUEBA DE BÚSQUEDA");
        
        // Test 4: Buscar archivos
        int foundId1 = fs.find("documento.txt");
        checkResult(foundId1 >= 0, "Encontrar 'documento.txt'");
        
        int foundId2 = fs.find("imagen.dat");
        checkResult(foundId2 >= 0, "Encontrar 'imagen.dat'");
        
        int notFound = fs.find("inexistente.txt");
        checkResult(notFound < 0, "No encontrar archivo inexistente");

        showSeparator("5. LISTADO DE ARCHIVOS");
        
        // Test 5: Listar archivos
        std::cout << "Contenido actual del filesystem:\n";
        fs.listFiles();

        showSeparator("6. PRUEBA DE ARCHIVOS GRANDES");
        
        // Test 6: Archivo grande (para probar bloques indirectos)
        std::string archivoGrande;
        for (int i = 0; i < 100; ++i) {
            archivoGrande += "Esta es la línea " + std::to_string(i) + " de un archivo muy grande para probar bloques indirectos.\n";
        }
        
        int idGrande = fs.create("archivo_grande.txt");
        checkResult(idGrande >= 0, "Crear archivo grande");
        
        bool writeGrande = fs.write("archivo_grande.txt", archivoGrande);
        checkResult(writeGrande, "Escribir archivo grande");
        
        std::string readGrande = fs.read("archivo_grande.txt");
        checkResult(readGrande == archivoGrande, "Leer archivo grande correctamente");
        
        std::cout << "Tamaño del archivo grande: " << archivoGrande.size() << " bytes\n";

        showSeparator("7. PRUEBA DE SOBRESCRITURA");
        
        // Test 7: Sobrescribir contenido
        std::string nuevoContenido = "Contenido completamente nuevo y diferente!";
        bool overwrite = fs.write("documento.txt", nuevoContenido);
        checkResult(overwrite, "Sobrescribir 'documento.txt'");
        
        std::string readOverwrite = fs.read("documento.txt");
        checkResult(readOverwrite == nuevoContenido, "Verificar sobrescritura");

        showSeparator("8. PRUEBA DE ELIMINACIÓN");
        
        // Test 8: Eliminar archivos
        std::cout << "Antes de eliminar:\n";
        fs.listFiles();
        
        bool deleteOk = fs.remove("imagen.dat");
        checkResult(deleteOk, "Eliminar 'imagen.dat'");
        
        // Verificar que ya no existe
        int deletedSearch = fs.find("imagen.dat");
        checkResult(deletedSearch < 0, "Archivo eliminado no se encuentra");
        
        std::string deletedRead = fs.read("imagen.dat");
        checkResult(deletedRead.empty(), "Archivo eliminado no se puede leer");

        showSeparator("9. ESTADO FINAL");
        
        std::cout << "Estado final del filesystem:\n";
        fs.listFiles();
        
        // Verificar integridad del filesystem
        checkResult(fs.isValid(), "Filesystem mantiene integridad");

        showSeparator("10. PRUEBA DE PERSISTENCIA");
        
        std::cout << "Los cambios se guardarán automáticamente al cerrar el filesystem.\n";
        std::cout << "Ejecuta el programa nuevamente para verificar persistencia.\n"; */
        fs.listFiles();
        fs.listFiles();
        fs.openFile("documento.txt");
        fs.write("documento.txt", "Probando el sistema de archivos");
        fs.closeFile("documento.txt");
        std::string contenido = fs.read("documento.txt");
        std::cout << "Contenido de 'documento.txt':\n" << contenido;
        fs.openFile("documento.txt");
        contenido = fs.read("documento.txt");
        std::cout << "Contenido de 'documento.txt':\n" << contenido << std::endl;
        fs.closeFile("documento.txt");
    } catch (const std::exception& e) {
        std::cerr << "ERROR CRÍTICO: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n¡TODAS LAS PRUEBAS COMPLETADAS!\n";
    std::cout << "El destructor guardará automáticamente todos los cambios.\n";
    
    return 0;
}

