#include "register.h"
#include "../model/auth.h"
#include "../protocol.h"
#include <string.h>
#include <unistd.h>

void handle_register(int client_socket, MYSQL *conn) {
    char username[50], password[50], buffer[BUFFER_SIZE];

    send(client_socket, "Username: ", strlen("Username: "), 0);
    read(client_socket, buffer, sizeof(buffer));
    strcpy(username, buffer);

    send(client_socket, "Password: ", strlen("Password: "), 0);
    read(client_socket, buffer, sizeof(buffer));
    strcpy(password, buffer);

    if (register_account(username, password, conn)) {
        send(client_socket, "Registration successful! Please log in.\n", 39, 0);
    } else {
        send(client_socket, "Registration failed. Username may already exist.\n", 49, 0);
    }
}
