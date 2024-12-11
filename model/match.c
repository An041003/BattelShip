#include <stdio.h>
#include <string.h>
#include <openssl/sha.h> 
#include <stdlib.h>
#include <mysql/mysql.h>
#include "../protocol.h"
#include "auth.h"
#include "match.h"
#include <pthread.h>

#define MAX_PLAYERS 10
Player waitingList[MAX_PLAYERS];
int waitingCount = 0;


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

    return matchId;
}

//sau khi trận đấu kết thúc hàm này cập nhập kết quả trận đấu vào database
// void update_match_elo(MYSQL *conn, int matchId, int  playerGrade1, int playerGrade2){
//     char query[256];
//     snprintf(query, sizeof(query), "UPDATE matches SET playerGrade1 = %d , playerGrade2 = %d WHERE id = %d",playerId1, playerId2, matchId );
//     if (mysql_query(conn, query)) {
//         fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
//     }
//     fprintf(stderr, "Updated\n");
// }
