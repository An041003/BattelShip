#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include <mysql/mysql.h>

typedef struct {
    int id;
    char player1[256];
    char player2[256];
    char winner[256];
} MatchData;

#define USER_DATA_FILE "user_data.txt"
MYSQL *connect_database();
bool register_account(const char *username, const char *password,MYSQL *conn);
bool login_account(const char *username, const char *password, MYSQL *conn);
bool username_exists(const char *username, MYSQL *conn);
void hashPassword(const char *password, char *hashed_password);
int get_player_id(char *username, MYSQL *conn);
int get_player_elo(int id, MYSQL *conn);
int getPlayerSocket(int playerId);
void addPlayerSocket(int playerId, int socket);
void update_elo(MYSQL *conn, int grade, int playerId);
char *get_user_name_by_id(int id, MYSQL *conn);
MatchData *get_player_matches(char *username, MYSQL *conn, int *match_count);

#endif 
