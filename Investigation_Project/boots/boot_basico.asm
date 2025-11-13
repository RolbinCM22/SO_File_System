; Minimal NASM boot sector that prints "hi!" then halts.
; Build: nasm -f bin boot.asm -o boot.bin
; Run:   qemu-system-i386 -drive format=raw,file=boot.bin

org 0x7c00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7c00
    sti

    mov si, msg
.print_loop:
    lodsb
    cmp al, 0
    je .done
    mov ah, 0x0E      ; BIOS teletype
    mov bh, 0
    int 0x10
    jmp .print_loop

.done:
    cli
.halt:
    hlt
    jmp .halt

msg db "hi!", 0

times 510 - ($-$$) db 0
dw 0xAA55