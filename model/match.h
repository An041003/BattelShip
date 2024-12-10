#ifndef MATCH_H
#define MATCH_H
#include <mysql/mysql.h>

typedef struct {
    char username[50];
    int id;
    int elo;
    bool isWaiting;
} Player;


void addPlayerToQueue(Player player);
Player* findOpponent(Player player);
void removePlayerFromQueue(Player* player);
void handleMatchmaking(int client_socket, Player player);

MYSQL *connect_database();
int create_match(MYSQL *conn, int playerId1, int playerId2);
void update_match_elo(MYSQL *conn, int matchId, int  playerGrade1, int playerGrade2);
int find_match(MYSQL *conn, int userId, int *queue, int size);

#endif