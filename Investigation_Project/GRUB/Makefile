ASM = nasm
CC = gcc
LD = ld
OBJCOPY = objcopy
QEMU = qemu-system-i386

CFLAGS  = -m32 -ffreestanding -O2 -Wall -Wextra -fno-builtin -fno-stack-protector -fno-pic
LDFLAGS = -m elf_i386 -T linker.ld -nostdlib

# Objetos (agregados vga/user/kstring)
OBJS = boot.o kernel.o isr80_stub.o syscall.o process.o user.o vga.o kstring.o kbd.o shell.o



all: kernel.elf

boot.o: boot_grub.asm
	$(ASM) -f elf32 boot_grub.asm -o boot.o

# IMPORTANTE: isr80_stub.S es GAS -> usar gcc, NO nasm
isr80_stub.o: isr80_stub.S
	$(CC) $(CFLAGS) -c isr80_stub.S -o isr80_stub.o

kbd.o: kbd.c kbd.h
	$(CC) $(CFLAGS) -c kbd.c -o kbd.o

shell.o: shell.c shell.h
	$(CC) $(CFLAGS) -c shell.c -o shell.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

syscall.o: syscall.c syscall.h
	$(CC) $(CFLAGS) -c syscall.c -o syscall.o

process.o: process.c process.h
	$(CC) $(CFLAGS) -c process.c -o process.o

user.o: user.c user.h
	$(CC) $(CFLAGS) -c user.c -o user.o

vga.o: vga.c vga.h
	$(CC) $(CFLAGS) -c vga.c -o vga.o

kstring.o: kstring.c
	$(CC) $(CFLAGS) -c kstring.c -o kstring.o

asm_lidt.o: asm_lidt.c
	$(CC) $(CFLAGS) -c asm_lidt.c -o asm_lidt.o

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS) -o kernel.elf

run: all
	grub-file --is-x86-multiboot kernel.elf && echo "✓ Kernel multiboot OK"
	$(QEMU) -kernel kernel.elf

clean:
	rm -f *.o *.elf *.iso
