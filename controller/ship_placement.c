#include "ship_placement.h"
#include <SDL2/SDL.h>
#include "../protocol.h"
#include "../model/network.h"
#include "../model/auth.h"

void handle_ship_placement(int board[GRID_SIZE][GRID_SIZE], Ship *ships, int *current_ship, int ship_sizes[], char *current_direction) {
    SDL_Event e;

    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            exit(0);
        } else if (e.type == SDL_MOUSEBUTTONDOWN && *current_ship < 4) {
            int x = (e.button.x - 50) / 60;
            int y = (e.button.y - 50) / 60;

            if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                Ship ship = {x, y, ship_sizes[*current_ship], *current_direction};
                if (is_valid_position(board, ship)) {
                    place_ship(board, ship);
                    ships[(*current_ship)++] = ship;
                }
            }
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_h) {
                *current_direction = 'H';
            } else if (e.key.keysym.sym == SDLK_v) {
                *current_direction = 'V';
            }
        }
    }
}

void send_ship_positions(int sock, int board[GRID_SIZE][GRID_SIZE]) {
    char buffer[BUFFER_SIZE] = {0};
    strcpy(buffer, "PLACE_SHIP");
    memcpy(&buffer[strlen("PLACE_SHIP")], board, GRID_SIZE * GRID_SIZE * sizeof(int));
    if (send(sock, buffer, sizeof(buffer), 0) == -1) {
        perror("Failed to send ship positions");
    } else {
        printf("Ship positions sent to server successfully.\n");
    }
    memset(buffer, 0, sizeof(buffer));
    if (recv(sock, buffer, sizeof(buffer), 0) > 0) {
        if (strncmp(buffer, "PLACE_SUCCESS", strlen("PLACE_SUCCESS")) == 0) {
            printf("Match has started! Get ready to play.\n");
        } else {
            printf("Unexpected server response: %s\n", buffer);
        }
    } else {
        perror("Failed to receive acknowledgment from server");
    }
}
