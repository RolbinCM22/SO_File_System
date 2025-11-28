#include "process.h"
#include "user.h"
#include "vga.h"
#include <string.h>

static pcb_t procs[MAX_PROCS];
static int next_pid = 1;

static int find_free_slot(void){
    for(int i=0;i<MAX_PROCS;i++) if(procs[i].state==PS_EMPTY) return i;
    return -1;
}

int proc_create(int program_id, int priority, const char* args, int owner_uid){
    if(owner_uid < 0) return -10; // requiere login

    int slot = find_free_slot();
    if (slot < 0) { printf("proc_create: no slots\n"); return -1; }

    procs[slot].pid = next_pid++;
    procs[slot].program_id = program_id;
    procs[slot].priority = priority;
    procs[slot].state = PS_READY;
    procs[slot].owner_uid = owner_uid;
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

static int can_manage(int caller_uid, pcb_t* p){
    if(!p) return 0;
    return (caller_uid >= 0) && (user_is_admin(caller_uid) || p->owner_uid == caller_uid);
}

int proc_exit(int pid, int caller_uid){
    pcb_t* p = proc_get(pid);
    if(!can_manage(caller_uid, p)) return -2;
    p->state = PS_TERMINATED;
    return 0;
}

int proc_kill(int pid, int caller_uid){
    // kill es como exit pero normalmente para procesos ajenos: exigimos admin
    pcb_t* p = proc_get(pid);
    if(!p) return -1;
    if(!user_is_admin(caller_uid)) return -3;
    p->state = PS_TERMINATED;
    return 0;
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

int proc_set_priority(int pid, int priority, int caller_uid){
    pcb_t* p = proc_get(pid);
    if(!p) return -1;

    // dueño puede ajustar un rango “normal”, admin cualquier cosa
    if(p->owner_uid == caller_uid){
        if(priority > 10) return -4;
    } else if(!user_is_admin(caller_uid)){
        return -2;
    }
    p->priority = priority;
    return 0;
}

int proc_msg_send(int dest_pid, const char* buffer, int caller_uid){
    pcb_t* dest = proc_get(dest_pid);
    if(!dest) return -1;

    // solo dueño->dueño o admin
    if(!(user_is_admin(caller_uid) || dest->owner_uid == caller_uid)) return -2;

    strncpy(dest->msg, buffer ? buffer : "", MSG_SIZE-1);
    dest->msg[MSG_SIZE-1]=0;
    dest->has_msg = 1;
    return 0;
}

pcb_t* proc_get(int pid){
    for(int i=0;i<MAX_PROCS;i++){
        if(procs[i].state!=PS_EMPTY && procs[i].pid==pid) return &procs[i];
    }
    return NULL;
}

void proc_list_debug(void){
    printf("PID\tUID\tPrio\tState\tHasMsg\tWaitingFor\n");
    for(int i=0;i<MAX_PROCS;i++){
        if(procs[i].state!=PS_EMPTY){
            printf("%d\t%d\t%d\t%d\t%d\t%d\n",
                procs[i].pid,
                procs[i].owner_uid,
                procs[i].priority,
                procs[i].state,
                procs[i].has_msg,
                procs[i].waiting_for
            );
        }
    }
}
