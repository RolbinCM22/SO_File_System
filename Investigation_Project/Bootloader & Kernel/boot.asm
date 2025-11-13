; ---- boot.asm ----
; Build: nasm -f bin boot.asm -o boot.bin
; Bootloader minimalista que carga kernel desde LBA=1 a 0x1000:0000 y salta.

BITS 16
org 0x7C00

; --- Constantes ---
%define KERNEL_SECTORS  1
%define KLOAD_SEG        0x1000
%define KLOAD_OFS        0x0000
%define MAGIC_DWORD      0x4B524F53   ; "KROS"
%define CODE_SEL         0x08
%define DATA_SEL         0x10

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    mov [drive], dl
    sti

    ; --- Verificar número mágico en primer sector ---
    mov ax, [boot_magic]
    cmp ax, MAGIC_DWORD & 0xFFFF
    jne fail
    mov ax, [boot_magic+2]
    cmp ax, MAGIC_DWORD >> 16
    jne fail

    ; --- Mostrar mensaje BIOS ---
    mov si, msg
.print:
    lodsb
    test al, al
    jz .read_kernel
    mov ah, 0x0E
    int 0x10
    jmp .print

.read_kernel:
    mov ax, KLOAD_SEG
    mov es, ax
    xor bx, bx

    mov ah, 0x02          ; Leer sectores (CHS)
    mov al, KERNEL_SECTORS
    xor ch, ch
    mov cl, 2             ; Sector 2 = LBA 1
    xor dh, dh
    mov dl, [drive]
    int 0x13
    jc disk_err

    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax
    jmp CODE_SEL:pm_entry

[BITS 32]
pm_entry:
    mov ax, DATA_SEL
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov fs, ax
    mov gs, ax
    mov esp, 0x0009FFF0

    jmp 0x00010000        ; Saltar al kernel cargado
.hang:
    hlt
    jmp .hang

[BITS 16]
fail:
    mov si, err1
    call print_string
    hlt
    jmp $

disk_err:
    mov si, err2
    call print_string
    hlt
    jmp $

print_string:
    lodsb
    test al, al
    jz .done
    mov ah, 0x0E
    int 0x10
    jmp print_string
.done:
    ret

; --- Datos 16-bit ---
drive:      db 0
boot_magic: dd MAGIC_DWORD
msg:        db "Cargando kernel...",13,10,0
err1:       db "Error: numero magico no coincide!",13,10,0
err2:       db "Error al leer el kernel!",13,10,0

; --- GDT mínima (32-bit plano) ---
[BITS 32]
gdt:
    dq 0x0000000000000000
    dq 0x00CF9A000000FFFF
    dq 0x00CF92000000FFFF
gdt_desc:
    dw gdt_end - gdt - 1
    dd gdt
gdt_end:

[BITS 16]
times 510-($-$$) db 0
dw 0xAA55
