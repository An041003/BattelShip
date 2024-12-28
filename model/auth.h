#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>
#include <mysql/mysql.h>

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

#endif 
