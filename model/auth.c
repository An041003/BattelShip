#include "auth.h"
#include <stdio.h>
#include <string.h>
#include <openssl/sha.h> 
#include <stdlib.h>
#include <mysql/mysql.h>
#include "../protocol.h"
#include "network.h"
#include "match.h"


MYSQL *connect_database() {
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return NULL;
    }
    if (mysql_real_connect(conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, DB_POST, NULL, 0) == NULL) {
        fprintf(stderr, "mysql_real_connect() failed\n");
        mysql_close(conn);
        return NULL;
    }
    return conn;
}

void hashPassword(const char *password, char *hashedPassword) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)password, strlen(password), hash);

    // Chuyển đổi kết quả băm sang dạng chuỗi hex
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(hashedPassword + (i * 2), "%02x", hash[i]);
    }
    hashedPassword[SHA256_DIGEST_LENGTH * 2] = '\0';  // Kết thúc chuỗi
}

bool username_exists(const char *username, MYSQL *conn) {
    char query[256];
    bool isExist = false;

    // Escape the username to prevent SQL injection
    char escapedUsername[128];
    mysql_real_escape_string(conn, escapedUsername, username, strlen(username));

    snprintf(query, sizeof(query), "SELECT id FROM users WHERE username='%s'", escapedUsername);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return false;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "Failed to store result: %s\n", mysql_error(conn));
        return false;
    }

    // Check if any rows exist
    if (mysql_num_rows(result) > 0) {
        isExist = true;
    }

    mysql_free_result(result);
    return isExist;
}

bool register_account(const char *username, const char *password, MYSQL *conn)
{
    bool registerSuccess = false;
    if (username_exists(username,conn))
    {
        return false; 
    }
     char hashedPassword[65];
     hashPassword(password, hashedPassword);  // Băm mật khẩu

    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO users (username, password) VALUES ('%s', '%s')", username, hashedPassword);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "User registration failed: %s\n", mysql_error(conn));
         registerSuccess = false;
    } else {
        printf("User registered successfully!\n");
         registerSuccess = true;
    }
    return registerSuccess;
}

bool login_account(const char *username, const char *password, MYSQL *conn)
{
    char  hashedPassword[65];
    bool authSuccess = false;
    hashPassword(password, hashedPassword);
    char query[256];
    snprintf(query, sizeof(query), "SELECT * FROM users WHERE username='%s' AND password='%s'", username, hashedPassword);

    if (mysql_query(conn, query)) {
        fprintf(stderr, "Authentication query failed: %s\n", mysql_error(conn));
        return 0;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (result == NULL) {
        fprintf(stderr, "mysql_store_result() failed: %s\n", mysql_error(conn));
        return 0;
    }

    if (mysql_num_rows(result) > 0) {
        printf("%s login successful!\n", username);
        authSuccess = true;
    } else {
        printf("Authentication failed! Incorrect username or password.\n");
    }

    mysql_free_result(result);
    return authSuccess; 

}

void addPlayerSocket(int playerId, int socket) {
    for (int i = 0; i < playerSocketCount; i++) {
        if (playerSockets[i].playerId == playerId) {
            playerSockets[i].socket = socket; // Cập nhật socket nếu đã tồn tại
            return;
        }
    }

    if (playerSocketCount < MAX_PLAYERS) {
        playerSockets[playerSocketCount].playerId = playerId;
        playerSockets[playerSocketCount].socket = socket;
        playerSocketCount++;
    } else {
        printf("Player socket storage is full.\n");
    }
}


int getPlayerSocket(int playerId) {
    for (int i = 0; i < playerSocketCount; i++) {
        if (playerSockets[i].playerId == playerId) {
            return playerSockets[i].socket;
        }
    }
    return -1; // Không tìm thấy
}


int get_player_id(char *username, MYSQL *conn){
    int id;
    char query[256];
    snprintf(query, sizeof(query), "SELECT id FROM users WHERE username = '%s'", username);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return false;
    }
    MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
        id = atoi(row[0]); // Convert the first column to an integer
    } else {
        fprintf(stderr, "No user found with username: %s\n", username);
    }
    mysql_free_result(result);
    return id;
    }

    int get_player_elo(int id, MYSQL *conn){
    int elo;
    char query[256];
    snprintf(query, sizeof(query), "SELECT elo FROM users WHERE id = '%d'", id);
     if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return false;
    }
     MYSQL_RES *result = mysql_store_result(conn);
    MYSQL_ROW row = mysql_fetch_row(result);
    if (row) {
    elo = atoi(row[0]); // Convert the first column to an integer
    } else {
        fprintf(stderr, "No user found with id: %d\n", id);
    }
    mysql_free_result(result);
     return elo;
}

int *get_player_matches(char *username, MYSQL *conn){
    int match_count;
   int userId = get_player_id(username, conn);
   char query[256];
   snprintf(query, sizeof(query), "SELECT id FROM matches WHERE playerId1 = '%d' or playerId2 = '%d'", userId);
     if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
        return false;
    }

    MYSQL_RES *result = mysql_store_result(conn);
    if (!result) {
        fprintf(stderr, "Could not retrieve result set: %s\n", mysql_error(conn));
        return NULL;
    }
    int num_rows = mysql_num_rows(result);
    int *matchIds = (int *)malloc(num_rows * sizeof(int));
    if (!matchIds) {
        fprintf(stderr, "Memory allocation failed\n");
        mysql_free_result(result);
        return NULL;
    }
    MYSQL_ROW row;
    int i = 0;
    while ((row = mysql_fetch_row(result))) {
        matchIds[i++] = atoi(row[0]);
    }
    mysql_free_result(result);
    return matchIds;
}

//hàm này cập nhập điểm của 1 người sau khi kết thúc một trận đấu
void update_elo(MYSQL *conn, int grade, int playerId){
    char query[256];
    snprintf(query, sizeof(query), "UPDATE users SET elo = grade + elo WHERE id = %d ",playerId, playerId);
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Authentication query failed: %s\n", mysql_error(conn));
    }
     fprintf(stderr, "Updated\n");
}


