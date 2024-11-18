#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include <mysql/mysql.h>

#define USER_DATA_FILE "user_data.txt"

bool register_account(const char *username, const char *password,MYSQL *conn);

bool login_account(const char *username, const char *password, MYSQL *conn);

void hashPassword(const char *password, char *hashed_password);

#endif 
