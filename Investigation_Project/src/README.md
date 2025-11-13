# MichiKernel - Investigation Project

## Descripción
Kernel básico que muestra un gato ASCII en pantalla usando modo protegido de 32 bits y VGA text mode.

## Requisitos del Sistema
- Linux (Ubuntu/Debian recomendado)
- QEMU para emulación
- GCC cross-compiler para i686
- NASM (ensamblador)
- Make

### Instalación de Dependencias (Ubuntu/Debian)
```bash
sudo apt update
sudo apt install build-essential nasm qemu-system-x86 gcc-multilib
```

## Compilación y Ejecución

### 1. Limpiar archivos previos
```bash
make clean
```
Elimina todos los archivos objeto (.o) y binarios generados anteriormente.

### 2. Compilar el proyecto
```bash
make
```
- Ensambla `boot.asm` → `boot.o`
- Compila `kernel.c` → `kernel.o` 
- Enlaza ambos → `kernel.bin`

### 3. Ejecutar en QEMU
```bash
make run
```
Lanza el kernel en el emulador QEMU mostrando:
- Mensaje de bienvenida
- Instrucciones
- Gato ASCII animado

### 4. Compilar y ejecutar (un solo comando)
```bash
make clean && make run
```

## Estructura del Proyecto
```
Investigation_Project/
├── src/
│   ├── boot.asm        # Bootloader en assembly
│   ├── kernel.c        # Kernel principal
│   ├── linker.ld       # Script de enlazado
│   ├── Makefile        # Automatización de build
│   └── README.md       # Este archivo
└── build/              # Archivos generados (auto-creado)
```

## Comandos Makefile Disponibles

| Comando | Descripción |
|---------|-------------|
| `make` | Compila todo el proyecto |
| `make clean` | Limpia archivos generados |
| `make run` | Ejecuta el kernel en QEMU |
