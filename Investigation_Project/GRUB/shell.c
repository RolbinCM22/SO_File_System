#include "shell.h"
#include "vga.h"
#include "kbd.h"
#include "syscalls_user.h"
#include "process.h"
#include <stdint.h>
#include <string.h>
#include "syscalls_user.h"
#include "../src/VMM_C/virtual_memory_unit.h"
#include "../src/VMM_C/physical_memory_manager.h"

// vienen de kernel.c
extern virtual_memory_unit_t g_vmu;
extern physical_memory_manager_t g_pmm;


/**
 * @brief Convert a string to an integer
 * 
 * @param s Input string
 * @return int Converted integer
 */
static int katoi(const char* s){
    int sign=1, x=0;
    if(!s) return 0;
    if(*s=='-'){ sign=-1; s++; }
    while(*s>='0' && *s<='9'){ x = x*10 + (*s - '0'); s++; }
    return sign*x;
}

/**
 * @brief Split a string into tokens based on spaces
 * 
 * @param line Input string to split
 * @param argv Array to store pointers to tokens
 * @param max Maximum number of tokens to split
 * @return int Number of tokens found
 */
static int split(char* line, char* argv[], int max){
    int n=0;
    while(*line){
        while(*line==' ') line++;
        if(!*line) break;
        if(n>=max) break;
        argv[n++] = line;
        while(*line && *line!=' ') line++;
        if(*line){ *line = 0; line++; }
    }
    return n;
}

/**
 * @brief Display the shell prompt
 * 
 */
static void prompt(void){
    console_set_color(0x0A);
    console_write("michi> ");
    console_set_color(0x0F);
}
/**
 * @brief Execute a shell command based on parsed arguments
 * 
 * @param argc Number of arguments
 * @param argv Array of argument strings
 */
static void run_cmd(int argc, char* argv[]){
    uint32_t regs[8];

    if(argc==0) return;

    if(strcmp(argv[0], "whoami")==0){
        regs[0]=SYS_WHOAMI;
        int uid = (int)isr80_c(regs);
        printf("uid=%d\n", uid);
        return;
    }

    if(strcmp(argv[0], "login")==0){
        if(argc<3){ console_writeln("uso: login <user> <pass>"); return; }
        regs[0]=SYS_LOGIN; regs[3]=(uint32_t)argv[1]; regs[2]=(uint32_t)argv[2];
        int r = (int)isr80_c(regs);
        if(r>=0) printf("login ok uid=%d\n", r);
        else console_writeln("login fallo");
        return;
    }

    if(strcmp(argv[0], "logout")==0){
        regs[0]=SYS_LOGOUT;
        isr80_c(regs);
        console_writeln("logout ok");
        return;
    }

    if(strcmp(argv[0], "useradd")==0){
        if(argc<4){ console_writeln("uso: useradd <user> <pass> <role:user|admin>"); return; }
        int role = 0;
        if(strcmp(argv[3], "admin")==0) role = 1;
        else role = 0;

        regs[0]=SYS_USER_ADD;
        regs[3]=(uint32_t)argv[1];
        regs[2]=(uint32_t)argv[2];
        regs[1]=(uint32_t)role;
        int uid = (int)isr80_c(regs);
        printf("useradd -> %d\n", uid);
        return;
    }

    if(strcmp(argv[0], "ps")==0){
        proc_list_debug();
        return;
    }

    if(strcmp(argv[0], "kill")==0){
        if(argc<2){ console_writeln("uso: kill <pid>"); return; }
        int pid = katoi(argv[1]);
        regs[0]=SYS_PROC_KILL; regs[3]=(uint32_t)pid;
        int r = (int)isr80_c(regs);
        printf("kill -> %d\n", r);
        return;
    }

    if(strcmp(argv[0], "prio")==0){
        if(argc<3){ console_writeln("uso: prio <pid> <n>"); return; }
        int pid = katoi(argv[1]);
        int pr  = katoi(argv[2]);
        regs[0]=SYS_SET_PRIORITY; regs[3]=(uint32_t)pid; regs[2]=(uint32_t)pr;
        int r = (int)isr80_c(regs);
        printf("prio -> %d\n", r);
        return;
    }
    if(strcmp(argv[0], "mkproc")==0){
        if(argc < 4){
            console_writeln("uso: mkproc <program_id> <prio> <name>");
            return;
        }
        int program_id = katoi(argv[1]);
        int prio       = katoi(argv[2]);
        const char* name = argv[3];

        uint32_t regs[8];
        regs[0] = SYS_CREATE_PROCESS;
        regs[3] = (uint32_t)program_id;   // arg0
        regs[2] = (uint32_t)prio;         // arg1
        regs[1] = (uint32_t)name;         // arg2 (args/name)
        int pid = (int)isr80_c(regs);
        printf("pid=%d\n", pid);
        return;
    }
    if(strcmp(argv[0], "vread") == 0){
        if(argc < 3){
            console_writeln("uso: vread <vpn> <off>");
            return;
        }

        int vpn = katoi(argv[1]);
        int off = katoi(argv[2]);

        char value = 0;
        int st = vmu_read_memory(&g_vmu, (size_t)vpn, (size_t)off, &value);

        if(st == 0){
            printf("vread vpn=%d off=%d -> %d (0x%x)\n", vpn, off, (int)(unsigned char)value, (unsigned char)value);
        } else {
            printf("vread error=%d (vpn/off fuera de rango)\n", st);
        }
        return;
    }
    if(strcmp(argv[0], "vwrite") == 0){
        if(argc < 4){
            console_writeln("uso: vwrite <vpn> <off> <val>");
            return;
        }

        int vpn = katoi(argv[1]);
        int off = katoi(argv[2]);
        int val = katoi(argv[3]);          // 0..255 recomendado

        int st = vmu_write_memory(&g_vmu, (size_t)vpn, (size_t)off, (char)val);

        if(st == 0){
            printf("vwrite ok vpn=%d off=%d val=%d\n", vpn, off, val);
        } else {
            printf("vwrite error=%d\n", st);
        }
        return;
    }
    if(strcmp(argv[0], "vframes") == 0){
        pmm_print_frame_table(&g_pmm);
        return;
    }

    console_writeln("comando no reconocido");
}

/**
 * @brief Run the shell interface
 * 
 */
void shell_run(void){
    char line[128];
    while(1){
        prompt();

        int len = 0;
        while(1){
            int ch = kbd_getchar_blocking();

            if(ch=='\n'){
                console_putc('\n');
                line[len]=0;
                break;
            }
            if(ch=='\b'){
                if(len>0){
                    len--;
                    console_putc('\b');
                }
                continue;
            }

            if(ch>=32 && ch<=126){
                if(len < (int)sizeof(line)-1){
                    line[len++] = (char)ch;
                    console_putc((char)ch);
                }
            }
        }

        char* argv[8];
        int argc = split(line, argv, 8);
        run_cmd(argc, argv);
    }
}
