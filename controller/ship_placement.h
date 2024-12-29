#ifndef SHIP_PLACEMENT_H
#define SHIP_PLACEMENT_H

#include "../model/board.h"
#include <SDL2/SDL.h>

void handle_ship_placement(int board[GRID_SIZE][GRID_SIZE], Ship *ships, int *current_ship, int ship_sizes[], char *current_direction);
void send_ship_positions(int sock, int board[GRID_SIZE][GRID_SIZE], SDL_Renderer *renderer);

#endif // SHIP_PLACEMENT_H
