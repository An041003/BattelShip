#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <mysql/mysql.h>
#include "auth.h" // Thêm thư viện xác thực

#define PORT 8080
#define MAX_PLAYERS 2
#define BUFFER_SIZE 1024

#define DB_HOST "localhost"
#define DB_USER "phuong"
#define DB_PASS "Tranphuong1253*"
#define DB_NAME "battle_ship"

MYSQL *connectDatabase() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return NULL;
    }
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return NULL;
    }
    return conn;
}

int main() {
     MYSQL *conn = connectDatabase();
    if (conn == NULL) {
        return 1;
    }

    int server_fd, new_socket, players[MAX_PLAYERS] = {0}, player_count = 0;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *welcome_message = "Welcome to Battleship Game!\n";

    // Tạo socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Gán địa chỉ cho socket
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Waiting for players...\n");

    while (player_count < MAX_PLAYERS) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            continue;
        }

        // Yêu cầu người dùng chọn "login" hoặc "register"
        send(new_socket, "Type 'login' to log in or 'register' to create a new account: ", strlen("Type 'login' to log in or 'register' to create a new account: "), 0);
        read(new_socket, buffer, BUFFER_SIZE);

        if (strncmp(buffer, "register", 8) == 0) {
            // Xử lý đăng ký
            send(new_socket, "Enter your desired username: ", strlen("Enter your desired username: "), 0);
            read(new_socket, buffer, BUFFER_SIZE);
            char username[50];
            strcpy(username, buffer);

            send(new_socket, "Enter your desired password: ", strlen("Enter your desired password: "), 0);
            read(new_socket, buffer, BUFFER_SIZE);
            char password[50];
            strcpy(password, buffer);

            if (register_account(username, password,conn)) {
                send(new_socket, "Registration successful! Please reconnect to log in.\n", strlen("Registration successful! Please reconnect to log in.\n"), 0);
            } else {
                send(new_socket, "Registration failed. Username may already exist.\n", strlen("Registration failed. Username may already exist.\n"), 0);
            }
            close(new_socket);
        } else if (strncmp(buffer, "login", 5) == 0) {
            // Xử lý đăng nhập
            send(new_socket, "Enter your username: ", strlen("Enter your username: "), 0);
            read(new_socket, buffer, BUFFER_SIZE);
            char username[50];
            strcpy(username, buffer);

            send(new_socket, "Enter your password: ", strlen("Enter your password: "), 0);
            read(new_socket, buffer, BUFFER_SIZE);
            char password[50];
            strcpy(password, buffer);

            char session_id[20];
            if (login_account(username, password, conn)) {
                printf("Player %d logged in successfully.\n", player_count + 1);
                players[player_count++] = new_socket;
                send(new_socket, welcome_message, strlen(welcome_message), 0);
            } else {
                send(new_socket, "Login failed. Disconnecting...\n", strlen("Login failed. Disconnecting...\n"), 0);
                close(new_socket);
            }
        } else {
            send(new_socket, "Invalid option. Disconnecting...\n", strlen("Invalid option. Disconnecting...\n"), 0);
            close(new_socket);
        }
    }
    printf("Both players connected. Game starting...\n");

    // Game logic sẽ thêm vào đây

    for (int i = 0; i < MAX_PLAYERS; i++) {
        close(players[i]);
    }
    close(server_fd);
    return 0;
}
