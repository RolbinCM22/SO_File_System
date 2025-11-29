ASM  = nasm
CC   = gcc
LD   = ld
QEMU = qemu-system-i386

# OJO: el Makefile está en GRUB/, el VMM está fuera: ../src/VMM_C
VMM_DIR = ../src/VMM_C

CFLAGS  = -m32 -ffreestanding -O2 -Wall -Wextra -fno-builtin -fno-stack-protector -fno-pic \
          -I. -I$(VMM_DIR)
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

# Objetos del kernel ubicados en GRUB/
KOBJS = boot.o kernel.o isr80_stub.o syscall.o process.o user.o vga.o kstring.o kbd.o shell.o

# Compilar automáticamente TODOS los .c que existan en el VMM_DIR (ruta relativa correcta)
VMM_SRCS = $(wildcard $(VMM_DIR)/*.c)
VMM_OBJS = $(patsubst $(VMM_DIR)/%.c,%.o,$(VMM_SRCS))

OBJS = $(KOBJS) $(VMM_OBJS)

.PHONY: all run clean

all: kernel.elf

boot.o: boot_grub.asm
	$(ASM) -f elf32 $< -o $@

# isr80_stub.S es GAS
isr80_stub.o: isr80_stub.S
	$(CC) $(CFLAGS) -c $< -o $@

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

syscall.o: syscall.c syscall.h
	$(CC) $(CFLAGS) -c $< -o $@

process.o: process.c process.h
	$(CC) $(CFLAGS) -c $< -o $@

user.o: user.c user.h
	$(CC) $(CFLAGS) -c $< -o $@

vga.o: vga.c vga.h
	$(CC) $(CFLAGS) -c $< -o $@

kstring.o: kstring.c
	$(CC) $(CFLAGS) -c $< -o $@

kbd.o: kbd.c kbd.h
	$(CC) $(CFLAGS) -c $< -o $@

shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -c $< -o $@

# Regla genérica: compila cualquier ../src/VMM_C/xxx.c a xxx.o en GRUB/
%.o: $(VMM_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o $@

run: all
	grub-file --is-x86-multiboot kernel.elf && echo "✓ Kernel multiboot OK"
	$(QEMU) -kernel kernel.elf

clean:
	rm -f *.o *.elf *.iso
