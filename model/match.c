#include <stdio.h>
#include <string.h>
#include <openssl/sha.h> 
#include <stdlib.h>
#include <mysql/mysql.h>
#include "../protocol.h"
#include "auth.h"

//hàm này tạo một trận đấu trong databases và trả về id của trận đó
int create_match(MYSQL *conn, int playerId1, int playerId2){
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
void update_match_elo(MYSQL *conn, int matchId, int  playerGrade1, int playerGrade2){
    char query[256];
    snprintf(query, sizeof(query), "UPDATE matches SET playerGrade1 = %d , playerGrade2 = %d WHERE id = %d",playerId1, playerId2, matchId );
    if (mysql_query(conn, query)) {
        fprintf(stderr, "Query failed: %s\n", mysql_error(conn));
    }
    fprintf(stderr, "Updated\n");
}

//hàm này nhận vào id của người đang tìm, một mảng chứa id những người đang đợi, số lượng người trong queue
//nếu ghép trận thành công sẽ tạo một trận trong db và trả về id của trận đấu đó, không thì trả về -1
int find_match(MYSQL *conn, int userId, int *queue, int size) {
    int elo = get_player_elo(userId, conn); 
    for (int i = size - 1; i >= 0; i--) {  
        int rivalElo = get_player_elo(queue[i], conn);
        if (abs(elo - rivalElo) <= 100) {  
            return create_match(conn, userId, queue[i]); 
        }
    }
    return -1; 
}
