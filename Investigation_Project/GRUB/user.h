#pragma once
#include <stdint.h>

#define MAX_USERS 8
#define USERNAME_MAX 16
#define PASSWORD_MAX 16

typedef enum { ROLE_USER=0, ROLE_ADMIN=1 } role_t;

typedef struct {
    int used;
    int uid;
    role_t role;
    char username[USERNAME_MAX];
    char password[PASSWORD_MAX]; // demo (sin hash real)
} user_t;
/**
 * @brief Initialize the user system
 * 
 */
void user_init(void);
/**
 * @brief Add a new user
 * 
 * @param u     Username of the new user
 * @param p     Password of the new user
 * @param role  Role of the new user (user or admin)
 * @return int  User ID of the newly added user, or -1 on failure
 */
int  user_add(const char* u, const char* p, role_t role);   // solo admin
/**
 * @brief Log in a user
 * 
 * @param u Username
 * @param p Password
 * @return int User ID if successful, -1 otherwise
 */
int  user_login(const char* u, const char* p);              // devuelve uid o -1
/**
 * @brief Log out the current user
 * 
 * @return int User ID of the logged out user, or -1 if no user was logged in
 */
int  user_logout(void);
/**
 * @brief Get the user ID of the currently logged in user
 * 
 * @return int User ID if logged in, -1 otherwise
 */
int  user_whoami(void);
/**
 * @brief Check if a user is an admin
 * 
 * @param uid User ID
 * @return int 1 if admin, 0 otherwise
 */
int  user_is_admin(int uid);
