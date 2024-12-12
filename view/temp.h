#ifndef TEMP_H
#define TEMP_H

#include <SDL2/SDL.h>
#include <stdbool.h>

// Kích thước lưới và các thông số
#define GRID_SIZE 10  // Số ô trên mỗi hàng và cột
#define CELL_SIZE 50  // Kích thước mỗi ô (pixel)
#define PADDING 50    // Khoảng cách giữa lưới và viền cửa sổ

/**
 * Hàm vẽ bàn cờ trống lên màn hình.
 * @param renderer: Con trỏ SDL_Renderer dùng để vẽ.
 * @param board: Ma trận chứa thông tin bàn cờ (trạng thái ô).
 *                - 0: Ô trống
 *                - 1: Ô trượt (miss)
 *                - 2: Ô trúng (hit)
 */
void render_empty_board(SDL_Renderer *renderer, int board[GRID_SIZE][GRID_SIZE]);

/**
 * Hàm hiển thị giao diện chính của game bắn tàu.
 * Người chơi có thể click chuột để đánh dấu "trúng" hoặc "trượt".
 * @param renderer: Con trỏ SDL_Renderer dùng để vẽ.
 * @param sock: Socket kết nối với server.
 */
void run_battleship_game(SDL_Renderer *renderer, int sock);

#endif // TEMP_H
