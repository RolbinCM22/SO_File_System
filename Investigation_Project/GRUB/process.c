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

int proc_wait(int waiter_pid, int target_pid){
    pcb_t* waiter = proc_get(waiter_pid);
    pcb_t* target = proc_get(target_pid);
    if (!waiter || !target) return -1;
    if (target->state == PS_TERMINATED) return 0; 
    waiter->state = PS_WAITING;
    waiter->waiting_for = target_pid;
    return 0;
}

int proc_set_priority(int pid, int priority){
    pcb_t* proc = proc_get(pid);
    if (!proc) return -1;
    proc->priority = priority;
    return 0;
}

int proc_msg_send(int dest_pid, const char* buffer){
    for(int i=0;i<MAX_PROCS;i++){
        if(procs[i].state!=PS_EMPTY && procs[i].pid==dest_pid){
            strncpy(procs[i].msg, buffer ? buffer : "", MSG_SIZE-1);
            procs[i].msg[MSG_SIZE-1]=0;
            procs[i].has_msg = 1;
            return 0;
        }
    }
    return -1;
}

pcb_t* proc_get(int pid){
    for(int i=0;i<MAX_PROCS;i++){
        if(procs[i].state!=PS_EMPTY && procs[i].pid==pid){
            return &procs[i];
        }
    }
    return NULL;
}

void proc_list_debug(void){
    printf("PID\tProgID\tPrio\tState\tHasMsg\tWaitingFor\n");
    for(int i=0;i<MAX_PROCS;i++){
        if(procs[i].state!=PS_EMPTY){
            printf("%d\t%d\t%d\t%d\t%d\t%d\n",
                procs[i].pid,
                procs[i].program_id,
                procs[i].priority,
                procs[i].state,
                procs[i].has_msg,
                procs[i].waiting_for
            );
        }
    }
}