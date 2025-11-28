#include <stdint.h>
#include "syscall.h"
#include "process.h"
#include "user.h"
#include "vga.h"
#include "shell.h"

#define VIDEO ((volatile char*)0xB8000)

void print(const char* s, uint8_t color, int row, int col) {
    int pos = (row * 80 + col) * 2;
    while (*s) {
        VIDEO[pos++] = *s++;
        VIDEO[pos++] = color;
    }
}

void kernel_main() {
    
    cls();
    user_init();

    puts_at(2, 30, "Bienvenido al MichiKernel", 0x1E);
    puts_at(3, 35, "Instrucciones", 0x2F);
    puts_at(5, 10, "Comandos: whoami | login u p | logout | useradd u p role | ps | kill pid | prio pid n", 0x0A);
    console_putc('\n');
    console_writeln("Shell listo. Proba: whoami");
    shell_run();

    uint32_t regs[8];

    // (e) SYS_USER_ADD: crear user normal "ana"
    regs[0]=SYS_USER_ADD; regs[3]=(uint32_t)"ana"; regs[2]=(uint32_t)"1234"; regs[1]=(uint32_t)ROLE_USER;
    int uid_ana = (int)isr80_c(regs);

    // logout root y login como ana
    regs[0]=SYS_LOGOUT; isr80_c(regs);
    regs[0]=SYS_LOGIN; regs[3]=(uint32_t)"ana"; regs[2]=(uint32_t)"1234";
    int login_uid = (int)isr80_c(regs);

    // whoami
    regs[0]=SYS_WHOAMI;
    int me = (int)isr80_c(regs);
    if(me>=0) puts_at(7, 5, "Login OK (ana)", 0x0F);

    // crear proceso A como ana
    regs[0]=SYS_CREATE_PROCESS; regs[3]=1; regs[2]=5; regs[1]=(uint32_t)"procA";
    int pidA = (int)isr80_c(regs);

    // intentar subir prioridad a 99 (debe fallar para user)
    regs[0]=SYS_SET_PRIORITY; regs[3]=pidA; regs[2]=99;
    int pr = (int)isr80_c(regs);
    if(pr<0) puts_at(9, 5, "Permiso denegado: prioridad alta (user)", 0x0E);

    // volver a root y matar procA con SYS_PROC_KILL (solo admin)
    regs[0]=SYS_LOGOUT; isr80_c(regs);
    regs[0]=SYS_LOGIN; regs[3]=(uint32_t)"root"; regs[2]=(uint32_t)"toor";
    isr80_c(regs);

    regs[0]=SYS_PROC_KILL; regs[3]=pidA;
    int k = (int)isr80_c(regs);
    if(k==0) puts_at(11, 5, "Root pudo matar procA (admin)", 0x0A);

    proc_list_debug();

    while(1);
}
