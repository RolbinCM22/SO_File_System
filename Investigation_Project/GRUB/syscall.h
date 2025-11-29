#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>

enum {
    SYS_CREATE_PROCESS = 1,
    SYS_EXIT_PROCESS   = 2,
    SYS_WAIT_PROCESS   = 3,
    SYS_SET_PRIORITY   = 4,
    SYS_MESSAGE_SEND   = 5,

    SYS_LOGIN          = 6,
    SYS_LOGOUT         = 7,
    SYS_WHOAMI         = 8,
    SYS_USER_ADD       = 9,
    SYS_PROC_KILL      = 10,

    SYS_VMM_READ       = 11,
    SYS_VMM_WRITE      = 12,
    SYS_VMM_FRAMES     = 13,
    SYS_PROC_LIST      = 14
};

uint32_t isr80_c(uint32_t *regs);

#endif

