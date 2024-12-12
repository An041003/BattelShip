#include "login.h"
#include "../model/auth.h"
#include "../protocol.h"
#include <string.h>
#include <unistd.h>
#include <stdio.h> 

void handle_login(int client_socket, MYSQL *conn) {
    char buffer[BUFFER_SIZE];
    char command[10], username[50], password[50];

    int bytes_received = read(client_socket, buffer, sizeof(buffer) - 1);
    if (bytes_received <= 0) {
        perror("Error reading from client");
        return;
    }
    buffer[bytes_received] = '\0'; 

    if (sscanf(buffer, "%s %s %s", command, username, password) != 3 || strcmp(command, "LOGIN") != 0) {
        send(client_socket, "Invalid command format.\n", 24, 0);
        return;
    }

    // Gọi hàm xử lý đăng ký
    if (login_account(username, password, conn)) {
        send(client_socket, "LOGIN", strlen("LOGIN"), 0); 
    } else {
        send(client_socket, "Username or Password incorrected.\n", 32, 0);
    }
}
void send_fire(int x, int y, int sock) {
    char fire_message[50];
    snprintf(fire_message, sizeof(fire_message), "FIRE|%d|%d", x, y);
    send(sock, fire_message, strlen(fire_message), 0);
}

