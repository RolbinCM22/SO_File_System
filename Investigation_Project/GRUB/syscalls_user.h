#pragma once
#include <stdint.h>
#include "syscall.h"   // <-- aquí está isr80_c

static inline int sc_whoami(){
  uint32_t r[8]={0}; r[0]=SYS_WHOAMI;
  return (int)isr80_c(r);
}

static inline int sc_login(const char* u, const char* p){
  uint32_t r[8]={0}; r[0]=SYS_LOGIN; r[3]=(uint32_t)u; r[2]=(uint32_t)p;
  return (int)isr80_c(r);
}

static inline int sc_logout(){
  uint32_t r[8]={0}; r[0]=SYS_LOGOUT;
  return (int)isr80_c(r);
}

static inline int sc_useradd(const char* u, const char* p, int role){
  uint32_t r[8]={0}; r[0]=SYS_USER_ADD; r[3]=(uint32_t)u; r[2]=(uint32_t)p; r[1]=(uint32_t)role;
  return (int)isr80_c(r);
}

static inline int sc_ps(){
  uint32_t r[8]={0}; r[0]=SYS_PROC_LIST;
  return (int)isr80_c(r);
}

static inline int sc_kill(int pid){
  uint32_t r[8]={0}; r[0]=SYS_PROC_KILL; r[3]=(uint32_t)pid;
  return (int)isr80_c(r);
}

static inline int sc_prio(int pid, int pr){
  uint32_t r[8]={0}; r[0]=SYS_SET_PRIORITY; r[3]=(uint32_t)pid; r[2]=(uint32_t)pr;
  return (int)isr80_c(r);
}

static inline int sc_mkproc(int program_id, int prio, const char* name){
  uint32_t r[8]={0}; r[0]=SYS_CREATE_PROCESS; r[3]=(uint32_t)program_id; r[2]=(uint32_t)prio; r[1]=(uint32_t)name;
  return (int)isr80_c(r);
}
