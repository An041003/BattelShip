#ifndef LOGIN_H
#define LOGIN_H

#include <mysql/mysql.h>

void handle_login(int client_socket, MYSQL *conn);

#endif