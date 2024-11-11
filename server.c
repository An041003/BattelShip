#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_PLAYERS 2
#define BUFFER_SIZE 1024

// Khởi tạo server
int main() {
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
        printf("Player %d connected.\n", player_count + 1);
        players[player_count++] = new_socket;
        send(new_socket, welcome_message, strlen(welcome_message), 0);
    }
    printf("Both players connected. Game starting...\n");

    // Game logic sẽ thêm vào đây

    for (int i = 0; i < MAX_PLAYERS; i++) {
        close(players[i]);
    }
    close(server_fd);
    return 0;
}
