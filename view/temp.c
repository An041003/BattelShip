#include <SDL2/SDL.h>
#include <stdbool.h>
#include "temp.h"
#include "fire_result_view.h"

// Hàm vẽ bàn cờ trống với các dấu hiệu "X" hoặc tròn nếu cần
void render_empty_board(SDL_Renderer *renderer, int board[GRID_SIZE][GRID_SIZE]) {
    // Đặt màu nền (màu đen)
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Đặt màu lưới (màu trắng)
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // Vẽ các đường lưới
    for (int i = 0; i <= GRID_SIZE; i++) {
        // Vẽ đường thẳng đứng
        SDL_RenderDrawLine(renderer, PADDING + i * CELL_SIZE, PADDING,
                           PADDING + i * CELL_SIZE, PADDING + GRID_SIZE * CELL_SIZE);

        // Vẽ đường thẳng ngang
        SDL_RenderDrawLine(renderer, PADDING, PADDING + i * CELL_SIZE,
                           PADDING + GRID_SIZE * CELL_SIZE, PADDING + i * CELL_SIZE);
    }

    // Vẽ các dấu hiệu trên bàn cờ
    for (int i = 0; i < GRID_SIZE; i++) {
        for (int j = 0; j < GRID_SIZE; j++) {
            int x = PADDING + j * CELL_SIZE;
            int y = PADDING + i * CELL_SIZE;

            if (board[i][j] == 1) {
                drawFireResult(renderer, x, y, CELL_SIZE, false); // Trượt
            } else if (board[i][j] == 2) {
                drawFireResult(renderer, x, y, CELL_SIZE, true);  // Trúng
            }
        }
    }

    // Hiển thị nội dung đã vẽ
    SDL_RenderPresent(renderer);
}

// Hàm chạy màn hình bắn tàu
void run_battleship_game(SDL_Renderer *renderer, int sock) {
    int board[GRID_SIZE][GRID_SIZE] = {0}; // Bàn cờ trống
    bool running = true;

    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                running = false;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                // Xử lý bắn: click trái là "trúng", click phải là "trượt"
                int x = (e.button.x - PADDING) / CELL_SIZE;
                int y = (e.button.y - PADDING) / CELL_SIZE;

                if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                    if (e.button.button == SDL_BUTTON_LEFT) {
                        board[y][x] = 2; // Đánh dấu trúng
                        send(sock, "HIT", strlen("HIT"), 0);
                    } else if (e.button.button == SDL_BUTTON_RIGHT) {
                        board[y][x] = 1; // Đánh dấu trượt
                        send(sock, "MISS", strlen("MISS"), 0);
                    }
                }
            }
        }

        // Hiển thị bàn cờ
        render_empty_board(renderer, board);
    }
}
