; [ECCI - UCR] Sistemas Operativos - 2025
; Sistema operativo de un solo sector
; Ensamble a binario: nasm -f bin boot.asm -o boot.bin
; Emule con:  qemu-system-i386 -drive format=raw,file=boot.bin -nographic

org 0x7c00

start:
    cli
    xor ax,ax

    ; Pila en SS:SP -> 0x0000:0x7C00 
    mov ss,ax
    mov sp,0x7c00
    ; DS y ES en 0 para etiquetas DS:SI = 0x0000:0x7C00.
    mov ds,ax
    mov es,ax
    sti
    call limpiarPantalla

main_loop:
    mov si, txtMenu
    call imprimirHilera
    call recibirTecla     
    cmp al, '1'
    je dibujeCuadrado
    cmp al, '2'
    je leerTexto
    cmp al, '3'
    je imprimaTexto
    cmp al, '4'
    je usuarioLimpiaPantalla
  cmp al, '5'
    je salirSO
    jmp main_loop

; -----------------------
; Opcion 1: Dibuje cuadrado

dibujeCuadrado:
    call limpiarPantalla
    mov si, txtTamanio
    call imprimirHilera
    call leerNumero   
    cmp ax, 1
    jl done_option
    cmp ax, 20
    jbe tamanio
    mov ax, 20
tamanio:
    mov bx, ax        
    mov cx, 0         
dibujeFila:
    cmp cx, bx
    jge done_option
    mov ah, 0x02
    mov bh, 0
    mov dh, 5
    add dh, cl
    mov dl, 10
    int 0x10
    mov di, bx        
dibujeCuadradoCiclo:
    cmp di, 0
    je nuevaFila
    mov al, '*'
    call imprimirChar
    dec di
    jmp dibujeCuadradoCiclo  
nuevaFila:
    call imprimirCRLF
    inc cx
    jmp dibujeFila

; -----------------------
; Opcion 2: Leer texto

leerTexto:
    mov si, txtEscriba
    call imprimirHilera
    push es
    mov ax, 0x0900
    mov es, ax
    xor di, di
    mov cx, 80        ; max chars
cicloLeerTexto:
    call recibirTecla  
    cmp al, 0x0D      ; Enter
    je finish_read
    cmp al, 0x08      ; Backspace
    jne guardarChar
    cmp di, 0
    je cicloLeerTexto
    dec di
    mov al, 0x08
    call imprimirChar
    mov al, ' '
    call imprimirChar
    mov al, 0x08
    call imprimirChar
    jmp cicloLeerTexto
guardarChar:
    push ax
    call imprimirChar
    pop ax
    mov [es:di], al
    inc di
    loop cicloLeerTexto
finish_read:
    ; terminate with zero
    mov byte [es:di], 0
    call imprimirCRLF
    pop es
    jmp main_loop

; -----------------------
; Opcion 3: Imprima texto

imprimaTexto:
    push ds
    mov ax, 0x0900
    mov ds, ax
    xor si, si
cicloImprimaTexto:
    mov al, [ds:si]
    cmp al, 0
    je finImprimaTexto
    call imprimirChar
    inc si
    jmp cicloImprimaTexto
finImprimaTexto:
    pop ds
    jmp done_option

; -----------------------
; Opcion 4: Limpiar pantalla
; -----------------------
usuarioLimpiaPantalla:
    call limpiarPantalla
    jmp main_loop

done_option:
    mov si, txtPresione
    call imprimirHilera
    call recibirTecla   
    jmp main_loop

; -----------------------
; Rutinas de BIOS

; imprimirChar: AL = char
imprimirChar:
    pusha
    mov ah, 0x0E
    mov bh, 0
    int 0x10
    popa
    ret

; imprimirHilera: DS:SI -> zero-terminated string
imprimirHilera:
    pusha
.imprima:
    lodsb
    cmp al, 0
    je .done
    call imprimirChar
    jmp .imprima
.done:
    popa
    ret

; imprimirCRLF
imprimirCRLF:
    mov al, 0x0D
    call imprimirChar
    mov al, 0x0A
    call imprimirChar
    ret

; recibirTecla: devuelve AL = ASCII
recibirTecla:
    mov ah, 0
    int 0x16
    ret

; leerNumero: lee hasta 2 digitos, devuelve en ax
leerNumero:
    xor bx, bx        ; BX = accumulator
    mov cx, 2         ; max 2 digits
.rn_loop:
    call recibirTecla
    cmp al, 0x0D
    je .rn_done
    cmp al, '0'
    jb .rn_loop
    cmp al, '9'
    ja .rn_loop
    ; echo digit
    push ax
    call imprimirChar
    pop ax
    ; acc = acc*10 + (al - '0')
    sub al, '0'
    mov ah, 0
    mov dx, bx
    shl bx, 1
    shl dx, 2
    add bx, dx        ; bx = bx * 10
    add bx, ax
    loop .rn_loop
.rn_done:
    mov ax, bx
    ret

limpiarPantalla:
    pusha
    mov ah, 0x06
    xor al, al    
    mov bh, 0x07  
    mov ch, 0
    mov cl, 0
    mov dh, 24
    mov dl, 79
    int 0x10
    popa
    ret

; -----------------------
; Data

txtMenu db 0x0D,0x0A
         db "Menu",0x0D,0x0A
         db "1)Figura",0x0D,0x0A
         db "2)Leer",0x0D,0x0A
         db "3)Ver",0x0D,0x0A
         db "4)Limpiar",0x0D,0x0A
         db "5)Salir",0x0D,0x0A
         db "SO: ",0

; -----------------------
; Opcion 5: Salir

salirSO:
    mov si, txtSalida
    call imprimirHilera
    cli
    hlt
    jmp $

txtSalida db 0x0D,0x0A, "SO: Fin....",0x0D,0x0A,0
txtTamanio db 0x0D,0x0A, "Lado 1-20: ",0
txtEscriba db 0x0D,0x0A, "Escriba: ",0
txtPresione db 0x0D,0x0A, "Teclazo..",0

    times 510 - ($-$$) db 0
    dw 0xAA55