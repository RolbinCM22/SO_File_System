#include "user.h"
#include "vga.h"
#include <string.h>

static user_t users[MAX_USERS];
static int next_uid = 1;
static int current_uid = -1;

void user_init(void){
    for(int i=0;i<MAX_USERS;i++) users[i].used=0;

    // root/admin
    users[0].used = 1;
    users[0].uid = 0;
    users[0].role = ROLE_ADMIN;
    strncpy(users[0].username, "root", USERNAME_MAX);
    strncpy(users[0].password, "toor", PASSWORD_MAX);

    current_uid = 0; // arrancás como root para demo (podés poner -1 si querés exigir login)
}

int user_is_admin(int uid){
    for(int i=0;i<MAX_USERS;i++)
        if(users[i].used && users[i].uid==uid) return users[i].role==ROLE_ADMIN;
    return 0;
}

int user_whoami(void){ return current_uid; }

int user_add(const char* u, const char* p, role_t role){
    if(!user_is_admin(current_uid)) return -1;

    // evitar duplicados
    for(int i=0;i<MAX_USERS;i++)
        if(users[i].used && strcmp(users[i].username, u)==0) return -2;

    for(int i=0;i<MAX_USERS;i++){
        if(!users[i].used){
            users[i].used = 1;
            users[i].uid = next_uid++;
            users[i].role = role;
            strncpy(users[i].username, u, USERNAME_MAX);
            strncpy(users[i].password, p, PASSWORD_MAX);
            return users[i].uid;
        }
    }
    return -3;
}

int user_login(const char* u, const char* p){
    for(int i=0;i<MAX_USERS;i++){
        if(users[i].used &&
           strcmp(users[i].username,u)==0 &&
           strcmp(users[i].password,p)==0){
            current_uid = users[i].uid;
            return current_uid;
        }
    }
    return -1;
}

int user_logout(void){
    current_uid = -1;
    return 0;
}
