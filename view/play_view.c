#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <fcntl.h>
#include <pthread.h>
#include "../controller/ship_placement.h"
#include "base_view.h"
#include "../model/network.h"
#include "../model/board.h"
#include "play_view.h"
#include <unistd.h>
#include "result_view.h"
#include "../model/auth.h"

void run_play_screen(SDL_Renderer *renderer, int sock, char buffer[256]) {
    // Khởi tạo SDL
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_Font *font = TTF_OpenFont("/home/an/Documents/GitHub/BattelShip/arial.ttf", 24);
    bool play = true;
    bool is_my_turn = false;
    bool show_message = false;
    char message[256];
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);
    int cell_status[GRID_SIZE][GRID_SIZE] = {0};
    //pthread_mutex_t data_mutex = PTHREAD_COND_INITIALIZER;
    while(play){
    // Vẽ giao diện
            SDL_Event event;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer);
            int player_grid_x = PADDING;
            int player_grid_y = PADDING;
            draw_text(renderer, "Oppent's Board", player_grid_x, player_grid_y - 50, (SDL_Color){255, 255, 255, 255}, font);
            for (int row = 0; row < GRID_SIZE; row++) {
            for (int col = 0; col < GRID_SIZE; col++) {
                SDL_Rect cell_rect = {player_grid_x + col * CELL_SIZE, player_grid_y + row * CELL_SIZE, CELL_SIZE, CELL_SIZE};
                if (cell_status[row][col] == 1) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Màu đỏ
                } else if (cell_status[row][col] == 2) {
                    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255); // Màu xám
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Màu xanh (mặc định)
                }
                SDL_RenderFillRect(renderer, &cell_rect);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
                SDL_RenderDrawRect(renderer, &cell_rect);
            }
        }
            if (strncmp(buffer, "YOUR_TURN", strlen("YOUR_TURN")) == 0 || strncmp(buffer, "Y", strlen("Y")) == 0) {
                is_my_turn = true;
                draw_text(renderer, "Your turn", 900, PADDING, (SDL_Color){0, 255, 0, 255}, font);
            } else if (strncmp(buffer, "OPPONENT_TURN", strlen("OPPONENT_TURN")) == 0 || strncmp(buffer, "O", strlen("O")) == 0) {
                is_my_turn = false;
                draw_text(renderer, "Wait opponent", 900, PADDING, (SDL_Color){255, 0, 0, 255}, font);
            } else if (strncmp(buffer, "HIT", strlen("HIT")) == 0) {
                int col, row;
                sscanf(buffer, "HIT %d %d", &col, &row);
                printf("%d %d\n",col,row);
                cell_status[row][col] = 1;
            } else if (strncmp(buffer, "MISS", strlen("MISS")) == 0) {
                int col, row;
                sscanf(buffer, "MISS %d %d", &col, &row);
                //printf("%d %d\n",col,row);
                cell_status[row][col] = 2;   
            } else if (strncmp(buffer, "SUNK", strlen("SUNK")) == 0) {
                draw_text(renderer, "You Lose", 900, PADDING + 50, (SDL_Color){255, 255, 0, 255}, font);
                MYSQL *conn = connect_database();
                update_elo( conn, -12, get_player_id(global_username, conn));
                mysql_close(conn);
                // display_defeated_screen(renderer, 1000-12);
                // usleep(100000);
                // char request[512];
                // snprintf(request, sizeof(request), "LOSE %s", global_username);
                // send(sock, request, strlen(request), 0);
                // printf("%s\n",request);
                break;
            } else if (strncmp(buffer, "DESTROY", strlen("DESTROY")) == 0) {
                draw_text(renderer, "You Win", 900, PADDING + 50, (SDL_Color){255, 255, 0, 255}, font);  
                MYSQL *conn = connect_database();
                update_elo( conn, 12, get_player_id(global_username, conn));
                mysql_close(conn);
                //display_victory_screen(renderer, 1012);
                // usleep(100000);
                // char request[512];
                // snprintf(request, sizeof(request), "WIN %s", global_username);
                // send(sock, request, strlen(request), 0);
                // printf("%s\n",request);
                break;
            }
            // else{ 
            //     printf("%s\n", buffer);
            // }
        
        memset(buffer, 0, sizeof(buffer));
        //pthread_mutex_lock(&data_mutex);
        recv(sock, buffer, sizeof(buffer), 0);
        //pthread_mutex_unlock(&data_mutex);
        SDL_Delay(500);
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
                    
                    
                    snprintf(message, sizeof(message), "FIRE %d %d ", col, row);
                    //show_message = true;
                    if (send(sock, message, strlen(message), 0) == -1) {
                        perror("Send failed");
                    } else {
                       draw_text(renderer, message, 900, PADDING, (SDL_Color){255, 255, 255, 255}, font);
                        is_my_turn = false;
                    }
                } else {
                    draw_text(renderer, "Wait opponent", 1100, PADDING, (SDL_Color){255, 255, 255, 255}, font);
                }
            }
        }
    }   
    
        SDL_RenderPresent(renderer);
    }
    
    // Giải phóng tài nguyên
    TTF_CloseFont(font);
}
