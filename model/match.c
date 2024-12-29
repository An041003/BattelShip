#include <stdio.h>
#include <string.h>
#include <openssl/sha.h> 
#include <stdlib.h>
#include <unistd.h>
#include <mysql/mysql.h>
#include "../protocol.h"
#include "auth.h"
#include "match.h"
#include <pthread.h>
#include "board.h"

Player waitingList[MAX_PLAYERS];
int waitingCount = 0;
int playerSocketCount = 0;
int match_count = 0;


void addPlayerToQueue(Player player) {

    for (int i = 0; i < waitingCount; i++) {
        if (strcmp(waitingList[i].username, player.username) == 0) {
            printf("Player %s is already in the queue.\n", player.username);
            return;
        }
    }

    if (waitingCount < MAX_PLAYERS) {
        waitingList[waitingCount++] = player;
        printf("Player %s added to queue.\n", player.username);
    } else {
        printf("Queue is full, cannot add player %s.\n", player.username);
    }
}

Player* findOpponent(Player player) {
    int candidateIndices[waitingCount]; 
    int candidateCount = 0;

    for (int i = 0; i < waitingCount; i++) {
        if (waitingList[i].username != NULL &&
            strcmp(waitingList[i].username, player.username) != 0 && 
            abs(waitingList[i].elo - player.elo) <= 100) {
            candidateIndices[candidateCount++] = i;
        }
    }

    if (candidateCount == 0) {
        return NULL;
    }

    int randomIndex = candidateIndices[rand() % candidateCount];
    return &waitingList[randomIndex];
}

void removePlayerFromQueue(Player* player) {
    for (int i = 0; i < waitingCount; i++) {
        if (strcmp(waitingList[i].username, player->username) == 0) {
            for (int j = i; j < waitingCount - 1; j++) {
                waitingList[j] = waitingList[j + 1];
            }
            waitingCount--;
            break;
        }
    }
}

void handleMatchmaking(int client_socket, Player player) {
    printf("Player %s is looking for a match...\n", player.username);

    while (1) {
        // Tìm đối thủ trong hàng chờ
        Player* opponent = findOpponent(player);

        if (opponent) {
            // Nếu tìm thấy đối thủ
            char response[BUFFER_SIZE];

            // Gửi thông tin đối thủ về client hiện tại
            snprintf(response, BUFFER_SIZE, "MATCH_FOUND %s %d", opponent->username, opponent->elo);
            send(client_socket, response, strlen(response), 0);
            // Xóa cả hai người chơi khỏi hàng chờ
            // removePlayerFromQueue(&player);
            removePlayerFromQueue(opponent);
            MYSQL *conn = connect_database();
            if (conn == NULL) {
            close(client_socket);
            return NULL;
            }
            int matchID = create_match(conn, get_player_id(player.username, conn), get_player_id(opponent->username, conn));
            if (matchID != -1)
            printf("Match %d: %s vs %s\n",matchID, player.username, opponent->username);
            mysql_close;
            break; // Thoát khỏi vòng lặp khi tìm được trận
        } else {
            // Nếu không tìm thấy đối thủ, giữ người chơi trong hàng chờ
            printf("No match found yet for %s, waiting...\n", player.username);
            sleep(5); // Đợi 5 giây trước khi kiểm tra lại
        }
        
    }
}

//hàm này tạo một trận đấu trong databases và trả về id của trận đó
int create_match(MYSQL *conn, int playerId1, int playerId2){
    if (playerId1 == playerId2) {
        return -1; 
    }
    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO matches (playerId1, playerId2) VALUES (%d, %d) ",playerId1, playerId2 );
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return -1;
    }
    int matchId = -1;
    matchId = (int)mysql_insert_id(conn);

    if (matchId == 0) {
        fprintf(stderr, "Failed to retrieve match ID.\n");
        return -1;
    }
    int player1_socket = getPlayerSocket(playerId1);
    int player2_socket = getPlayerSocket(playerId2);
    if (match_count >= MAX_MATCHES) {
        printf("Match limit reached.\n");
        return -1;
    }

    matches[match_count].player1_socket = player1_socket;
    matches[match_count].player2_socket = player2_socket;
    matches[match_count].player1_ready = 0;
    matches[match_count].player2_ready = 0;
    match_count = match_count +1 ;

    if (player1_socket == -1 || player2_socket == -1) {
        printf("Failed to retrieve sockets for players.\n");
        return -1;
    }

    printf("Match created: %d vs %d\n", player1_socket, player2_socket);
    return matchId;
}

void update_match_outcome(MYSQL *conn, int matchId, char *winner){
    char query[256];
    snprintf(query, sizeof(query), "UPDATE matches SET Winner = %s WHERE id = %d",winner, matchId );
     if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return -1;
    }
    fprintf(stderr, "update winner successfull!\n");
}


void match_control(int socket1, int socket2){

    int board1[GRID_SIZE][GRID_SIZE];
    int board2[GRID_SIZE][GRID_SIZE];
    memcpy(board1, temp_match.board1, sizeof(board1));
    memcpy(board2, temp_match.board2, sizeof(board2));
    char buffer[BUFFER_SIZE];
    Ship ship1, ship2;
    int current_player = 1;
    send(temp_match.player1_socket,YOUR_TURN, strlen(YOUR_TURN), 0);
    send(temp_match.player2_socket,OPPONENT_TURN, strlen(OPPONENT_TURN), 0);
    char your[BUFFER_SIZE];
    strcpy(your, "YOUR_TURN\n");
    char op[BUFFER_SIZE];
    strcpy(op, "OPPONENT_TURN\n");
    while(1){
    int opponent = (current_player == 1) ? temp_match.player2_socket: temp_match.player1_socket;
    int current_client = (current_player == 1) ? temp_match.player1_socket: temp_match.player2_socket;
    int (*opponent_board)[GRID_SIZE]; 
    opponent_board = (current_player == 1) ? board2 : board1;
    Ship opponent_ship =  (current_player == 1) ? ship2 : ship1;
    printf("Now %d turn\n", current_client);
    //notify_turn(current_client, opponent );
    
    recv(current_client, buffer, sizeof(buffer), 0); 

    int row, col;
    sscanf(buffer, "FIRE %d %d", &row, &col);
    if(is_valid_move(row, col, opponent_board)) {
        int result = fire_result(row, col,opponent_board, opponent_ship);
        switch (result)
        {
        case 1: //  bắn trúng 1 ô tàu
            send(current_client,HIT, strlen(HIT), 0);
            send(opponent,HIT, strlen(HIT), 0);
            //printf("Sended\n");
            update_game_state(opponent, opponent_board);
            current_player = (current_player == 1) ? 2 : 1;
            break;
        case 2: // bắn chìm tàu rồi
        // báo cho ng chơi là họ thắng
            send(current_client,"WIN\n", BUFFER_SIZE, 0);
        //báo cho đối thủ là bị chìm tàu
            send(opponent,"SUNK\n", BUFFER_SIZE, 0);
            break;
        case 3: // bắn ko trúng
        
            send(current_client, MISS, strlen(MISS), 0);
            usleep(1000000);
            send(current_client, op, strlen(op), 0);
            send(opponent, MISS, strlen(MISS), 0);
            usleep(1000000);
            send(opponent, your, strlen(your), 0);
            update_game_state(opponent, opponent_board);
            current_player = (current_player == 1) ? 2 : 1;

            break;
        default:
            break;
        }
        
       
    }
    // printf("Now %d turn\n", current_client);
     
    // send(current_client,YOUR_TURN, strlen(YOUR_TURN), 0);
    // usleep(1000000);
    // send(opponent,OPPONENT_TURN, strlen(OPPONENT_TURN), 0);
    // printf("Sended %d turn\n", current_client);
    //notify_turn( current_client, opponent );
    
    }

}
int is_valid_move(int row, int col, int board[GRID_SIZE][GRID_SIZE]) {
    return (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE );
}
// hàm này gửi lại trạng thái ô cờ
void update_game_state(int client_socket,  int board[GRID_SIZE][GRID_SIZE]) {
    char buffer[BUFFER_SIZE] = {0x03};
    send(client_socket,"UPDATE\n", BUFFER_SIZE, 0);
    memcpy(&buffer[1], board, GRID_SIZE); 
    send(client_socket, buffer, BUFFER_SIZE, 0);
    memset(buffer, 0, BUFFER_SIZE);
}
void notify_turn(int client1_socket,int client2_socket) { 
    send(client1_socket,YOUR_TURN, strlen(YOUR_TURN), 0);
    send(client2_socket,OPPONENT_TURN, strlen(OPPONENT_TURN), 0);
    printf("Client %d turn\n", client1_socket);
}


//kiểm tra vị trí đặt có vào thuyền không
int fire_result( int row, int col, int board[GRID_SIZE][GRID_SIZE], Ship ship ){
    int hit = 1, sunk = 2, miss = 3;
    if(board[row][col] == 1){
        board[row][col] = 0;
        if(&ship.direction == "H"){
            for(int i = ship.x; i < ship.x + ship.length; i++){
                if(board[i][ship.y] == 1) {
                    return  hit ;
                }
            }
            return sunk ;
        }
        if(&ship.direction == "V"){
            for(int j = ship.y; j < ship.y + ship.length; j++){
                if(board[ship.x][j] == 1){
                    return hit;
                }
            }
            return sunk ;
        }
    }
     return miss;
}

void update_client_state(int client_socket, int in_match) {
    for (int i = 0; i < MAX_PLAYERS; i++) {
        if (client_states[i].socket == client_socket) {
            client_states[i].in_match = in_match;
            printf("Updated client %d to in_match = %d\n", client_socket, in_match);
            break;
        }
    }
}