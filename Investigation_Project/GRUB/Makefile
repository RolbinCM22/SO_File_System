ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy
QEMU = qemu-system-i386

CFLAGS = -m32 -ffreestanding -O2 -Wall -Wextra
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

all: kernel.elf

boot.o: boot_grub.asm
	$(ASM) -f elf32 boot_grub.asm -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel.elf: boot.o kernel.o
	$(LD) $(LDFLAGS) boot.o kernel.o -o kernel.elf

run: all
	grub-file --is-x86-multiboot kernel.elf && echo "✓ Kernel multiboot OK"
	qemu-system-i386 -kernel kernel.elf

clean:
	rm -f *.o *.elf
