#ifndef REGISTER_H
#define REGISTER_H

#include <mysql/mysql.h>

void handle_register(int client_socket, MYSQL *conn);

#endif