section .multiboot
align 4
    dd 0x1BADB002          ; Magic number (identifica kernel multiboot)
    dd 0x00                ; Flags
    dd -(0x1BADB002 + 0x00); Checksum = -(magic + flags)

section .text
global start
extern kernel_main

start:
    call kernel_main
.hang:
    hlt
    jmp .hang