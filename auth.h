#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

#define USER_DATA_FILE "user_data.txt"

bool register_account(const char *username, const char *password);

bool login_account(const char *username, const char *password, char *session_id);

void hash_password(const char *password, char *hashed_password);

#endif 
