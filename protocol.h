#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdbool.h> 

#define PORT 8000
#define BUFFER_SIZE 1024

#define DB_HOST "battle-ship-phuongxxx971022-4df8.c.aivencloud.com"
#define DB_USER "avnadmin"
#define DB_PASS "AVNS_lECCc4owrRuPx3Zjr0e"
#define DB_NAME "battle_ship"
#define DB_POST 16578

// Đăng ký/Đăng nhập
#define REGISTER "REGISTER"
#define REGISTER_SUCCESS "REGISTER_SUCCESS"
#define REGISTER_FAIL "REGISTER_FAIL"
#define LOGIN "LOGIN"
#define LOGIN_SUCCESS "LOGIN_SUCCESS"
#define LOGIN_FAIL "LOGIN_FAIL"

// Ghép trận
#define FIND_MATCH "FIND_MATCH"
#define MATCH_FOUND "MATCH_FOUND"
#define MATCH_WAITING "MATCH_WAITING"
#define OPPONENT_FOUND "OPPONENT_FOUND" 


// Đặt tàu
#define PLACE_SHIP "PLACE_SHIP"
#define PLACE_SUCCESS "PLACE_SUCCESS"
#define PLACE_FAIL "PLACE_FAIL"
#define ALL_SHIPS_PLACED "ALL_SHIPS_PLACED" 

// Sẵn sàng
#define READY "READY"
#define BOTH_PLAYERS_READY "BOTH_PLAYERS_READY"
#define START_GAME "START_GAME"


// Bắn
#define FIRE "FIRE"
#define FIRE_RESULT "FIRE_RESULT"
#define HIT "HIT"
#define MISS "MISS"
#define SUNK "SUNK"

// Lượt chơi
#define YOUR_TURN "YOUR_TURN"
#define OPPONENT_TURN "OPPONENT_TURN"

//update trạng thái sau mỗi lượt bắn
#define UPDATE "UPDATE"

// Tạm dừng/Thua/Hòa
#define PAUSE "PAUSE"
#define PAUSE_SUCCESS "PAUSE_SUCCESS"
#define PAUSE_FAIL "PAUSE_FAIL"
#define SURRENDER "SURRENDER"
#define SURRENDER_ACCEPTED "SURRENDER_ACCEPTED"
#define DRAW_OFFER "DRAW_OFFER" // Đề nghị hòa
#define DRAW_ACCEPT "DRAW_ACCEPT" // Chấp nhận hòa
#define DRAW_DECLINE "DRAW_DECLINE" // Từ chối hòa



// Kết thúc game
#define GAME_END "GAME_END"
#define WIN "WIN"
#define LOSE "LOSE"
#define DRAW "DRAW"

// Cập nhật điểm
#define ELO_UPDATE "ELO_UPDATE"

// Xếp hạng
#define RANKING "RANKING"
#define RANKING_RESULT "RANKING_RESULT"

// Lỗi
#define ERROR "ERROR" // Gửi khi có lỗi chung

// Thoát game
#define QUIT_GAME "QUIT_GAME"



#endif