#include "board.h"
void init_board(int board[GRID_SIZE][GRID_SIZE]) {
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            board[i][j] = 0;
        }
    }
}

bool is_valid_position(int board[GRID_SIZE][GRID_SIZE], Ship ship) {
    for (int i = 0; i < ship.length; i++) {
        int x = ship.x + (ship.direction == 'H' ? i : 0);
        int y = ship.y + (ship.direction == 'V' ? i : 0);

        if (x >= GRID_SIZE || y >= GRID_SIZE || board[y][x] != 0) {
            return false;
        }
    }
    return true;
}

void place_ship(int board[GRID_SIZE][GRID_SIZE], Ship ship) {
    for (int i = 0; i < ship.length; i++) {
        int x = ship.x + (ship.direction == 'H' ? i : 0);
        int y = ship.y + (ship.direction == 'V' ? i : 0);
        board[y][x] = 1;
    }
}

