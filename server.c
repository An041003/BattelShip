#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "protocol.h"
#include "model/auth.h"
#include "controller/register.h"
#include "controller/login.h"

// Hàm xử lý từng client trong một thread riêng
void *client_handler(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    MYSQL *conn = connect_database();

    if (conn == NULL) {
        send(client_socket, "Database connection failed. Disconnecting...\n", 45, 0);
        close(client_socket);
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    
    while (1) {
        memset(buffer, 0, sizeof(buffer));
        
        if (read(client_socket, buffer, sizeof(buffer)) <= 0) {
            printf("Client disconnected.\n");
            break;
        }

        if (strncmp(buffer, REGISTER, strlen(REGISTER)) == 0) {
            send(client_socket, "REGISTER.\n", 18, 0);
            handle_register(client_socket, conn);
        } else if (strncmp(buffer, LOGIN, strlen(LOGIN)) == 0) {
            send(client_socket, "LOGIN.\n", 18, 0);
            handle_login(client_socket, conn);
        } else if (strncmp(buffer, FIND_MATCH, strlen(FIND_MATCH)) == 0) {
            send(client_socket, "Matchmaking feature not implemented yet.\n", 42, 0);
        } else {
            send(client_socket, "Invalid command.\n", 18, 0);
        }
    }

    mysql_close(conn);
    close(client_socket);
    return NULL;
}


int main() {
    int server_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    

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

    if (listen(server_fd, 10) < 0) { // Lắng nghe tối đa 10 kết nối đang chờ
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server running on port %d\n", PORT);

    while (1) {
        int *client_socket = malloc(sizeof(int));
        if ((*client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("Accept failed");
            free(client_socket);
            continue;
        }

        printf("New client connected.\n");

        // Tạo một thread mới để xử lý client
        pthread_t thread_id;
        if (pthread_create(&thread_id, NULL, client_handler, client_socket) != 0) {
            perror("Failed to create thread");
            free(client_socket);
            continue;
        }

        // Detach thread để tự động thu hồi tài nguyên sau khi kết thúc
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}
