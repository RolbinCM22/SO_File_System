#include "process.h"
#include <string.h>

static pcb_t procs[MAX_PROCS];
static int next_pid = 1;

static int find_free_slot(void){
    for(int i=0;i<MAX_PROCS;i++) if(procs[i].state==PS_EMPTY) return i;
    return -1;
}

int proc_create(int program_id, int priority, const char* args){
    int slot = find_free_slot();
    if (slot < 0) {
        printf("proc_create: No free process slots available\n");    
        return -1; 
    }
    procs[slot].pid = next_pid++;
    procs[slot].program_id = program_id;
    procs[slot].priority = priority;
    procs[slot].state = PS_READY;
    procs[slot].has_msg = 0;
    procs[slot].waiting_for = -1;
    if (args) {
        strncpy(procs[slot].msg, args, MSG_SIZE - 1);
        procs[slot].msg[MSG_SIZE - 1] = '\0';
        procs[slot].has_msg = 1;
    } else {
        procs[slot].msg[0] = '\0';
    }
    return procs[slot].pid;
}

int proc_exit(int pid){
    for(int i=0;i<MAX_PROCS;i++){
        if(procs[i].pid==pid && procs[i].state!=PS_EMPTY){
            procs[i].state = PS_TERMINATED;
            return 0;
        }
    }
    return -1; 
}