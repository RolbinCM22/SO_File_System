#include <stdint.h>
#include "syscall.h"   // <-- aquí está isr80_c
#include "process.h"
#include "user.h"
#include "vga.h"
#include "shell.h"
#include "../src/VMM_C/virtual_memory_unit.h"
#include "../src/VMM_C/backing_store_manager.h"
#include "../src/VMM_C/physical_memory_manager.h"
#include "../src/VMM_C/page_fault_handler.h"
#include "../src/VMM_C/page_table_manager.h"
backing_store_manager_t g_bsm;
physical_memory_manager_t g_pmm;
page_fault_handler_t g_pfh;
page_table_manager_t g_ptm;
virtual_memory_unit_t g_vmu;

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
    puts_at(5, 10, "Comandos basicos:", 0x0A);
    puts_at(6, 12, "- whoami: muestra usuario actual", 0x07);
    puts_at(7, 12, "- login <user> <pass>: inicia sesion", 0x07);
    puts_at(8, 12, "- logout: cierra sesion", 0x07);
    puts_at(9, 12, "- useradd <u> <p> <rol>: crea usuario", 0x07);
    puts_at(10, 12, "- ps: lista procesos (PID, estado)", 0x07);
    puts_at(11, 12, "- kill <pid>: termina proceso", 0x07);
    puts_at(12, 12, "- prio <pid> <n>: cambia prioridad", 0x07);
    puts_at(13, 12, "- help: vuelve a mostrar esta lista", 0x07);
    puts_at(15, 10, "Memoria virtual:", 0x0A);
    puts_at(16, 12, "- vread <vpn> <off>", 0x07);
    puts_at(17, 12, "- vwrite <vpn> <off> <val>", 0x07);
    puts_at(18, 12, "- vframes", 0x07);
   
    bsm_init(&g_bsm);
   // bsm_init_test_patterns(&g_bsm);   

    pmm_init(&g_pmm);

    pfh_init(&g_pfh, &g_pmm, &g_bsm);
    ptm_init(&g_ptm, &g_pfh);

    vmu_init(&g_vmu, &g_pmm, &g_bsm);
    console_putc('\n');
    console_writeln("[KERNEL] Virtual Memory Unit initialized.");
    console_set_cursor(19, 0);
    shell_run();   // <- aquí el control NO vuelve
    while(1);
}
