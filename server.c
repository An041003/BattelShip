#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "protocol.h"
#include "model/auth.h"
#include "model/board.h"
#include "controller/register.h"
#include "controller/login.h"
#include "model/match.h"
#include "view/play_view.h"

// Hàm xử lý từng client trong một thread riêng
void *client_handler(void *arg) {
    ClientState *client_state = (ClientState *)arg;
    int client_socket = client_state->socket;
    client_state->in_match = 0;
    int board[GRID_SIZE][GRID_SIZE];
    MYSQL *conn = connect_database();

    if (conn == NULL) {
        send(client_socket, "Database connection failed. Disconnecting...\n", 45, 0);
        close(client_socket);
        return NULL;
    }

    char buffer[BUFFER_SIZE];
    
    while (1) {

        memset(buffer, 0, sizeof(buffer));
        
        if (client_state->in_match == 1) {
            sleep(1); // Chờ trạng thái thay đổi
            continue;
        }

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
            char username[50];
            sscanf(buffer, "FIND_MATCH %s", username);
            int player_id = get_player_id(username, conn);
            int elo = get_player_elo(player_id, conn);
            Player player = { .id = player_id, .elo = elo };
            strncpy(player.username, username, sizeof(player.username));
            addPlayerSocket(player_id, client_socket);
            addPlayerToQueue(player);
            handleMatchmaking(client_socket, player);
        } else if (strncmp(buffer, PLACE_SHIP, strlen(PLACE_SHIP)) == 0) {
            
            for (int i = 0; i < match_count; i++) {
        if (matches[i].player1_socket == client_socket) {
            // Cập nhật board của player1
            memcpy(matches[i].board1, &buffer[strlen(PLACE_SHIP)], GRID_SIZE * GRID_SIZE * sizeof(int));
            matches[i].player1_ready = 1;
            client_state->in_match = 1;
            printf("Player 1 board updated for match %d\n", i);

        } else if (matches[i].player2_socket == client_socket) {
            // Cập nhật board của player2
            memcpy(matches[i].board2, &buffer[strlen(PLACE_SHIP)], GRID_SIZE * GRID_SIZE * sizeof(int));
            matches[i].player2_ready = 1;
            client_state->in_match = 1;
            printf("Player 2 board updated for match %d\n", i);
        }

        // Nếu cả hai người chơi đã gửi board
        if (matches[i].player1_ready == 1 && matches[i].player2_ready == 1) {
            printf("Both boards are ready for match %d\n", i);
            memcpy(&temp_match, &matches[i], sizeof(temp_match));
            match_control(temp_match.player1_socket, temp_match.player2_socket);
            client_state->in_match = 0;
            // Reset 
            matches[i].player1_ready = 0;
            matches[i].player2_ready = 0;
        }

        break;
        }
        
        // }else if (strncmp(buffer, FIRE, strlen(FIRE)) == 0) {
        //         printf(" Boom %s\n", buffer);
                
        }else {
            printf("Invalid message.\n");
        }
    }

    mysql_close(conn);
    close(client_socket);
    free(client_socket);
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
    *client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);

    if (*client_socket < 0) {
        perror("Accept failed");
        free(client_socket);
        continue;
    }

    

    // Tạo ClientState
    ClientState *client_state = malloc(sizeof(ClientState));
    client_state->socket = *client_socket;
    client_state->in_match = 0;
    printf("New client %d connected state %d .\n", client_state->socket, client_state->in_match);
    pthread_t thread_id;
    if (pthread_create(&thread_id, NULL, client_handler, client_state) != 0) {
        perror("Failed to create thread");
        free(client_state);
        continue;
    }

    pthread_detach(thread_id);
}


    close(server_fd);
    return 0;
}
