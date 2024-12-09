#include "register.h"
#include "../model/auth.h"
#include "../protocol.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h> 

void handle_register(int client_socket, MYSQL *conn) {
    char buffer[BUFFER_SIZE];
    char command[10], username[50], password[50];

    int bytes_received = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_received <= 0) {
        perror("Error reading from client");
        return;
    }
    buffer[bytes_received] = '\0'; 

    if (sscanf(buffer, "%s %s %s", command, username, password) != 3 || strcmp(command, "REGISTER") != 0) {
        send(client_socket, "Invalid command format.\n", 24, 0);
        return;
    }

    // Gọi hàm xử lý đăng ký
    if (register_account(username, password, conn)) {
        send(client_socket, "REGISTER", strlen("REGISTER"), 0); 
    } else {
        send(client_socket, "ERROR: Username already exists.\n", 32, 0);
    }
}
