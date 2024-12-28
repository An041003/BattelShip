#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <fcntl.h>
#include "../controller/ship_placement.h"
#include "base_view.h"
#include "../model/network.h"
#include "../model/board.h"
#include "play_view.h"

void run_play_screen(SDL_Renderer *renderer, int sock) {
    // Khởi tạo SDL
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_Font *font = TTF_OpenFont("/home/an/Documents/GitHub/BattelShip/arial.ttf", 24);
    bool play = true;
    bool is_my_turn = false;
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    while(play){
        // Vẽ giao diện
        SDL_Event event;
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        int player_grid_x = PADDING;
        int player_grid_y = PADDING;
        draw_text(renderer, "Oppent's Board", player_grid_x, player_grid_y - 50, (SDL_Color){255, 255, 255, 255}, font);
        draw_grid(renderer, font, player_grid_x, player_grid_y);
        
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes_received > 0) {
            if (strncmp(buffer, "YOUR_TURN", strlen("YOUR_TURN")) == 0) {
                is_my_turn = true;
                draw_text(renderer, "Your turn!", 900, PADDING, (SDL_Color){0, 255, 0, 255}, font);
            } else if (strncmp(buffer, "OPPONENT_TURN", strlen("OPPONENT_TURN")) == 0) {
                is_my_turn = false;
                draw_text(renderer, "Wait opponent", 900, PADDING, (SDL_Color){255, 0, 0, 255}, font);
            }
        }
        // Xử lý sự kiện
         while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            play = false;
        } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int mouse_x = event.button.x;
            int mouse_y = event.button.y;

            int col = (mouse_x - player_grid_x) / CELL_SIZE;
            int row = (mouse_y - player_grid_y) / CELL_SIZE;

            if (col >= 0 && col < GRID_SIZE && row >= 0 && row < GRID_SIZE) {
                if (is_my_turn) {
                    char message[256];
                    snprintf(message, sizeof(message), "FIRE %d,%d", col, row);

                    if (send(sock, message, strlen(message), 0) == -1) {
                        perror("Send failed");
                    } else {
                        draw_text(renderer, message, 900, PADDING, (SDL_Color){255, 255, 255, 255}, font);
                        is_my_turn = false;
                    }
                } else {
                    draw_text(renderer, "Wait opponent", 900, PADDING, (SDL_Color){255, 255, 255, 255}, font);
                }
            }
        }
    }
        SDL_RenderPresent(renderer);
    }
    // Giải phóng tài nguyên
    TTF_CloseFont(font);
}
