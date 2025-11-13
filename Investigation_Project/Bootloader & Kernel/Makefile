# ============================================
#  Mini SO - Bootloader + Kernel (32 bits)
#  Autor: Rolbin Castillo Matamoros
# ============================================

# Herramientas
ASM     = nasm
CC      = gcc
LD      = ld
OBJCOPY = objcopy
QEMU    = qemu-system-i386

# Archivos
BOOT_BIN   = boot.bin
KERNEL_BIN = kernel.bin
KERNEL_OBJ = kernel.o
KERNEL_ELF = kernel.elf
DISK_IMG   = disk.img
LINKER     = linker.ld
SRC_KERNEL = kernel.c
SRC_BOOT   = boot.asm

# Configuración
KERNEL_SECTORS = 16
DISK_SIZE      = 4096      # sectores (2MB aprox)

# Opciones del compilador y linker
CFLAGS  = -m32 -ffreestanding -c
LDFLAGS = -m elf_i386 -T $(LINKER) -nostdlib

# ============================================

all: $(DISK_IMG)

$(BOOT_BIN): $(SRC_BOOT)
	@echo "[ASM] Ensamblando bootloader..."
	$(ASM) -f bin $(SRC_BOOT) -o $(BOOT_BIN)

$(KERNEL_OBJ): $(SRC_KERNEL)
	@echo "[CC] Compilando kernel..."
	$(CC) $(CFLAGS) $(SRC_KERNEL) -o $(KERNEL_OBJ)

$(KERNEL_ELF): $(KERNEL_OBJ)
	@echo "[LD] Enlazando kernel..."
	$(LD) $(LDFLAGS) $(KERNEL_OBJ) -o $(KERNEL_ELF)

$(KERNEL_BIN): $(KERNEL_ELF)
	@echo "[OBJCOPY] Generando kernel plano..."
	$(OBJCOPY) -O binary $(KERNEL_ELF) $(KERNEL_BIN)

$(DISK_IMG): $(BOOT_BIN) $(KERNEL_BIN)
	@echo "[DD] Creando imagen de disco..."
	dd if=/dev/zero of=$(DISK_IMG) bs=512 count=$(DISK_SIZE) status=none
	dd if=$(BOOT_BIN) of=$(DISK_IMG) bs=512 seek=0 conv=notrunc status=none
	dd if=$(KERNEL_BIN) of=$(DISK_IMG) bs=512 seek=1 conv=notrunc status=none
	@echo "[OK] Imagen creada: $(DISK_IMG)"

# ============================================

run: all
	@echo "[QEMU] Ejecutando sistema operativo..."
	$(QEMU) -drive format=raw,file=$(DISK_IMG) -boot c -no-reboot -no-shutdown

clean:
	rm -f $(BOOT_BIN) $(KERNEL_BIN) $(KERNEL_ELF) $(KERNEL_OBJ) $(DISK_IMG)
	@echo "[CLEAN] Archivos eliminados."

# ============================================
