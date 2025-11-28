#pragma once
#include <stdint.h>

#define MAX_PROCS 16
#define MSG_SIZE 128

typedef enum { PS_EMPTY=0, PS_READY=1, PS_RUNNING=2, PS_WAITING=3, PS_TERMINATED=4 } proc_state_t;

typedef struct {
    int pid;
    int program_id;
    int priority;
    proc_state_t state;
    char msg[MSG_SIZE];
    int has_msg;
    int waiting_for; /* pid this proc is waiting for (-1 none) */
} pcb_t;

int proc_create(int program_id, int priority, const char* args);
int proc_exit(int pid);
int proc_wait(int waiter_pid, int target_pid);
int proc_set_priority(int pid, int priority);
int proc_msg_send(int dest_pid, const char* buffer);
pcb_t* proc_get(int pid);
void proc_list_debug(void);