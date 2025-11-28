    .section .text
    .globl isr80_stub
    .type isr80_stub, @function
    .extern isr80_c

# 
isr80_stub:
    push %ebp
    mov  %esp, %ebp
    push %ebx
    push %esi
    push %edi

    push 8(%ebp)      # regs pointer
    call isr80_c
    add  $4, %esp

    pop  %edi
    pop  %esi
    pop  %ebx
    pop  %ebp
    ret
