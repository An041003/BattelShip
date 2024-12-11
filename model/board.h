#ifndef BOARD_H
#define BOARD_H

#include <stdbool.h>

#define GRID_SIZE 10

typedef struct {
    int x, y, length;
    char direction; // 'H' for horizontal, 'V' for vertical
} Ship;

void init_board(int board[GRID_SIZE][GRID_SIZE]);
bool is_valid_position(int board[GRID_SIZE][GRID_SIZE], Ship ship);
void place_ship(int board[GRID_SIZE][GRID_SIZE], Ship ship);

#endif // BOARD_H
