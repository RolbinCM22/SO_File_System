; matrix_boot.asm - Bootloader estilo "Matrix"
; Ensamblar con: nasm -f bin matrix_boot.asm -o matrix_boot.bin

bits 16
org 0x7C00

start:
    cli
    xor ax, ax
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0x7C00
    sti

    ; Poner modo texto 80x25
    mov ax, 0x0003
    int 0x10

    ; Segmento de video texto
    mov ax, 0xB800
    mov es, ax

    ; Limpiar pantalla
    xor di, di
    mov cx, 80*25
    mov ax, 0x0720          ; espacio, gris sobre negro
clear_loop:
    stosw
    loop clear_loop

    ; Semilla "random"
    mov si, 12345

main_loop:
    ; --- LCG simple para "random" ---
    mov ax, si
    mov bx, 25173
    mul bx                  ; DX:AX = AX*BX
    add ax, 13849
    mov si, ax              ; nueva semilla

    ; fila: 0..24
    mov bx, ax
    and bl, 0x1F            ; 0..31
    cmp bl, 25
    jb row_ok
    sub bl, 25
row_ok:

    ; columna: 0..79
    mov dx, ax
    shr dx, 8               ; usar parte alta
    mov cl, 80
col_mod:
    cmp dl, cl
    jb col_ok
    sub dl, cl
    jmp col_mod
col_ok:

    ; offset = (fila*80 + col)*2
    mov ah, 0
    mov al, bl              ; fila
    mov cl, 80
    mul cl                  ; AX = fila*80
    add al, dl              ; + columna
    adc ah, 0
    shl ax, 1               ; *2 (dos bytes por celda)
    mov di, ax

    ; caracter: algo tipo 0-9, A-Z
    mov ax, si
    and al, 0x1F            ; 0..31
    add al, '0'
    cmp al, '9'
    jbe char_ok
    add al, 7               ; saltar a letras mayúsculas
char_ok:
    ; atributo: verde brillante sobre negro = 0x0A
    mov ah, 0x0A

    mov [es:di], ax         ; escribir en pantalla

    jmp main_loop

; Rellenar con ceros hasta el final del sector
times 510-($-$$) db 0
dw 0xAA55                   ; firma de boot