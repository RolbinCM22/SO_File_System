section .text
    global _start

_start:
    ; Simulate loading bar
    mov ecx, 20         ; Number of loading steps
loading_loop:
    push ecx            ; Save loop counter

    ; Write one dot for loading bar
    mov eax, 4          ; sys_write
    mov ebx, 1          ; stdout
    mov ecx, dot_msg    ; message
    mov edx, 1          ; length
    int 0x80

    ; Sleep simulation (tight loop delay)
    mov ebp, 0x1FFFFFF  ; Delay counter
delay_loop:
    dec ebp
    jnz delay_loop

    pop ecx             ; Restore loop counter
    loop loading_loop

    ; Print newline after loading
    mov eax, 4
    mov ebx, 1
    mov ecx, newline
    mov edx, 1
    int 0x80

    ; Print menu
    mov eax, 4
    mov ebx, 1
    mov ecx, menu
    mov edx, menu_len
    int 0x80

    ; Exit program
    mov eax, 1
    xor ebx, ebx
    int 0x80

section .data
    dot_msg db '.'
    newline db 0xA       ; Newline character
    menu db '=== MAIN MENU ===', 0xA
         db '1. Option One', 0xA
         db '2. Option Two', 0xA
         db '3. Exit', 0xA
         db 'Choose: '
    menu_len equ $ - menu

    ; Padding to ensure exact 512-byte file size
    padding_times equ 512 - (menu_len + menu - $) - 14
    padding db padding_times dup(0)

section .bss
    ; No uninitialized data needed
