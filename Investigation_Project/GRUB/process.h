#pragma once
#include <stdint.h>

#define MAX_PROCS 16
#define MSG_SIZE  64

typedef enum { PS_EMPTY=0, PS_READY=1, PS_WAITING=2, PS_TERMINATED=3 } pstate_t;

typedef struct {
    int pid;
    int program_id;
    int priority;
    pstate_t state;

    int owner_uid;        // <-- NUEVO
    int has_msg;
    int waiting_for;
    char msg[MSG_SIZE];
} pcb_t;
/**
 * @brief Create a new process
 * 
 * @param program_id    Program identifier
 * @param priority Priority of the process
 * @param args Arguments for the process
 * @param owner_uid Owner user ID
 * @return int Process ID
 */
int   proc_create(int program_id, int priority, const char* args, int owner_uid);
/**
 * @brief Terminate a process
 * 
 * @param pid Process ID
 * @param caller_uid Caller user ID
 * @return int Result code
 */
int   proc_exit(int pid, int caller_uid);
/**
 * @brief Kill a process
 * 
 * @param pid Process ID
 * @param caller_uid Caller user ID
 * @return int Result code
 */
int   proc_kill(int pid, int caller_uid);
/**
 * @brief Wait for a process to change state
 * 
 * @param waiter_pid Process ID of the waiter
 * @param target_pid Process ID to wait for
 * @return int Result code
 */
int   proc_wait(int waiter_pid, int target_pid);
/**
 * @brief   Set the priority of a process
 * 
 * @param pid Process ID
 * @param priority New priority value
 * @param caller_uid Caller user ID
 * @return int Result code
 */
int   proc_set_priority(int pid, int priority, int caller_uid);
/**
 * @brief   Send a message to a process
 * 
 * @param dest_pid Destination process ID
 * @param buffer Message buffer
 * @param caller_uid Caller user ID
 * @return int Result code
 */
int   proc_msg_send(int dest_pid, const char* buffer, int caller_uid);
/**
 * @brief Get a process control block
 * 
 * @param pid Process ID
 * @return pcb_t* Pointer to the process control block
 */
pcb_t* proc_get(int pid);
/**
 * @brief List all processes for debugging
 * 
 */
void  proc_list_debug(void);
