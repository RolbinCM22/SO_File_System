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
    puts_at(5, 10, "Comandos: whoami | login u p | logout | useradd u p role | ps | kill pid | prio pid n", 0x0A);
    // Globals: g_bsm, g_pmm, g_pfh, g_ptm, g_vmu
    bsm_init(&g_bsm);
    bsm_init_test_patterns(&g_bsm);   // opcional pero útil para pruebas

    pmm_init(&g_pmm);

    pfh_init(&g_pfh, &g_pmm, &g_bsm);
    ptm_init(&g_ptm, &g_pfh);

    vmu_init(&g_vmu, &g_pmm, &g_bsm);
    console_putc('\n');
    console_writeln("Shell listo. Proba: whoami");
    console_set_cursor(10, 0);
    shell_run();   // <- aquí el control NO vuelve
    while(1);
}
