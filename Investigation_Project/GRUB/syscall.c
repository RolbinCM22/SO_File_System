#include "syscall.h"
#include "process.h"
#include "user.h"
#include <stdint.h>
/**
 * @brief Handle system calls
 * 
 * @param regs Pointer to the registers array containing syscall parameters
 * @return uint32_t Result of the system call
 */
uint32_t isr80_c(uint32_t *regs){
    uint32_t nr = regs[0];
    uint32_t a = regs[3];
    uint32_t b = regs[2];
    uint32_t c = regs[1];

    int uid = user_whoami();

    switch(nr){
        case SYS_CREATE_PROCESS:{
            int program_id = (int)a;
            int priority = (int)b;
            const char* args = (const char*)c;
            regs[0] = (uint32_t) proc_create(program_id, priority, args, uid);
            break;
        }
        case SYS_EXIT_PROCESS:{
            regs[0] = (uint32_t) proc_exit((int)a, uid);
            break;
        }
        case SYS_PROC_KILL:{
            regs[0] = (uint32_t) proc_kill((int)a, uid);
            break;
        }
        case SYS_WAIT_PROCESS:{
            regs[0] = (uint32_t) proc_wait((int)a, (int)b);
            break;
        }
        case SYS_SET_PRIORITY:{
            regs[0] = (uint32_t) proc_set_priority((int)a, (int)b, uid);
            break;
        }
        case SYS_MESSAGE_SEND:{
            regs[0] = (uint32_t) proc_msg_send((int)a, (const char*)b, uid);
            break;
        }

        case SYS_LOGIN:{
            regs[0] = (uint32_t) user_login((const char*)a, (const char*)b);
            break;
        }
        case SYS_LOGOUT:{
            regs[0] = (uint32_t) user_logout();
            break;
        }
        case SYS_WHOAMI:{
            regs[0] = (uint32_t) user_whoami();
            break;
        }
        case SYS_USER_ADD:{
            regs[0] = (uint32_t) user_add((const char*)a, (const char*)b, (role_t)c);
            break;
        }

        default:
            regs[0] = (uint32_t)-1;
            break;
    }
    return regs[0];
}
