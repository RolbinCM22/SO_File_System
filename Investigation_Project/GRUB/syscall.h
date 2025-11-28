#pragma once
#include <stdint.h>
/**
 * @brief System call numbers
 * 
 */
enum {
    SYS_CREATE_PROCESS = 1,
    SYS_EXIT_PROCESS   = 2,
    SYS_WAIT_PROCESS   = 3,
    SYS_SET_PRIORITY   = 4,
    SYS_MESSAGE_SEND   = 5,

    // (e) nuevas 5 syscalls
    SYS_LOGIN          = 6,  // (char* user, char* pass)
    SYS_LOGOUT         = 7,  // ()
    SYS_WHOAMI         = 8,  // ()
    SYS_USER_ADD       = 9,  // (char* user, char* pass, role)
    SYS_PROC_KILL      = 10  // (pid)
};

uint32_t isr80_c(uint32_t *regs);
