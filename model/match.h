#ifndef MATCH_H
#define MATCH_H
#include <mysql/mysql.h>
#define MAX_MATCHES 10
#define MAX_PLAYERS 100
#include "board.h"

typedef struct {
    int socket;
    int in_match; // 1 nếu đang trong trận đấu, 0 nếu không
} ClientState;

ClientState client_states[MAX_PLAYERS];

void add_client_state(int client_socket);
typedef struct {
    int playerId;
    int socket;
} PlayerSocket;

PlayerSocket playerSockets[MAX_PLAYERS];

typedef struct {
    int player1_socket;
    int player2_socket;
    int player1_ready;
    int player2_ready;
    int board1[GRID_SIZE][GRID_SIZE];
    int board2[GRID_SIZE][GRID_SIZE];
} Match;

Match matches[MAX_MATCHES];
Match temp_match;
void match_control(int socket1, int socket2);
void another_control(int socket1, int socket2);

typedef struct {
    char username[50];
    int id;
    int elo;
    bool isWaiting;
    int sock;
} Player;

extern int playerSocketCount; 
extern int match_count; 
extern int match_temp;
void addPlayerToQueue(Player player);
Player* findOpponent(Player player);
void removePlayerFromQueue(Player* player);
void handleMatchmaking(int client_socket, Player player);

MYSQL *connect_database();
int create_match(MYSQL *conn, int playerId1, int playerId2);
void update_match_elo(MYSQL *conn, int matchId, int  playerGrade1, int playerGrade2);
int find_match(MYSQL *conn, int userId, int *queue, int size);
void update_match_outcome(MYSQL *conn, int matchId, char *winner);
#endif