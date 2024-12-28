#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "../controller/ship_placement.h"
#include "base_view.h"
#include "../model/network.h"
#include "play_view.h"

void run_place_ship_screen(SDL_Renderer *renderer, int sock) {
    // Khởi tạo SDL
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_Font *font = TTF_OpenFont("/home/an/Documents/GitHub/BattelShip/arial.ttf", 24);
    SDL_Texture *ship_texture = IMG_LoadTexture(renderer, "/home/an/Documents/GitHub/BattelShip/ship.webp");

    // Kiểm tra nếu font hoặc texture không được tải
    if (!font || !ship_texture) {
        printf("Failed to load resources: %s\n", SDL_GetError());
        return;
    }

    // Khởi tạo board và các biến liên quan
    int board[GRID_SIZE][GRID_SIZE];
    init_board(board);

    Ship ships[4];
    int ship_sizes[4] = {5, 4, 3, 2};
    int current_ship = 0;
    char current_direction = 'H';

    // Vị trí và màu sắc nút "Ready"
    SDL_Rect ready_button = {1600 - 1000, 900 - 100, 120, 50};
    SDL_Color button_color = {0, 128, 0, 255}; // Màu xanh lá cây
    bool placing_ships = true;


    // Vòng lặp chính để đặt tàu
    while (placing_ships) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                placing_ships = false;
                break;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = (e.button.x - PADDING) / CELL_SIZE;
                int y = (e.button.y - PADDING) / CELL_SIZE;

                // Kiểm tra nếu click vào lưới để đặt tàu
                if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE && current_ship < 4) {
                    Ship ship = {x, y, ship_sizes[current_ship], current_direction};
                    if (is_valid_position(board, ship)) {
                        place_ship(board, ship);
                        ships[current_ship++] = ship;
                    }
                }

                // Kiểm tra nếu nhấn vào nút "Ready"
                if (current_ship == 4 &&
                    e.button.x >= ready_button.x && e.button.x <= ready_button.x + ready_button.w &&
                    e.button.y >= ready_button.y && e.button.y <= ready_button.y + ready_button.h) {
                    send_ship_positions(sock, board);
                    run_play_screen(renderer, sock);
                    placing_ships = false;
                }
            } else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_h) {
                    current_direction = 'H';
                } else if (e.key.keysym.sym == SDLK_v) {
                    current_direction = 'V';
                }
            }
        }

        // Vẽ giao diện
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        int player_grid_x = PADDING;
        int player_grid_y = PADDING;
        draw_text(renderer, "Your Board", player_grid_x, player_grid_y - 50, (SDL_Color){255, 255, 255, 255}, font);
        draw_grid(renderer, font, player_grid_x, player_grid_y);

        for (int i = 0; i < current_ship; i++) {
            draw_ship(renderer, ship_texture, ships[i]);
        }

        draw_instructions(renderer, font, current_direction, current_ship < 4 ? ship_sizes[current_ship] : 0);

        // Vẽ nút "Ready" nếu đã đặt xong tất cả tàu
        if (current_ship == 4) {
            draw_button(renderer, ready_button, button_color, "Ready", font);
        }

        SDL_RenderPresent(renderer);
    }

    // Giải phóng tài nguyên
    SDL_DestroyTexture(ship_texture);
    TTF_CloseFont(font);
}
