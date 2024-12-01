#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define GRID_SIZE 10
#define CELL_SIZE 60
#define PADDING 50

typedef struct {
    int x, y, length;
    char direction; // 'H' for horizontal, 'V' for vertical
} Ship;

// Vẽ lưới với ký hiệu
void draw_grid(SDL_Renderer *renderer, TTF_Font *font) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Màu xanh dương cho lưới

    for (int i = 0; i <= GRID_SIZE; i++) {
        // Đường ngang
        SDL_RenderDrawLine(renderer, PADDING, PADDING + i * CELL_SIZE, PADDING + GRID_SIZE * CELL_SIZE, PADDING + i * CELL_SIZE);
        // Đường dọc
        SDL_RenderDrawLine(renderer, PADDING + i * CELL_SIZE, PADDING, PADDING + i * CELL_SIZE, PADDING + GRID_SIZE * CELL_SIZE);
    }

    // Vẽ ký hiệu cột (1-10) và hàng (A-J)
    SDL_Color white = {255, 255, 255, 255};
    for (int i = 0; i < GRID_SIZE; i++) {
        char label[2];
        snprintf(label, sizeof(label), "%d", i + 1);

        SDL_Surface *surface = TTF_RenderText_Solid(font, label, white);
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

        SDL_Rect dest = {PADDING + i * CELL_SIZE + CELL_SIZE / 2 - surface->w / 2, PADDING - 30, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &dest);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);

        snprintf(label, sizeof(label), "%c", 'A' + i);

        surface = TTF_RenderText_Solid(font, label, white);
        texture = SDL_CreateTextureFromSurface(renderer, surface);

        dest = (SDL_Rect){PADDING - 40, PADDING + i * CELL_SIZE + CELL_SIZE / 2 - surface->h / 2, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &dest);

        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    }
}

// Vẽ tàu trên bảng
void draw_ship(SDL_Renderer *renderer, SDL_Texture *ship_texture, Ship ship) {
    SDL_Rect dest;

    for (int i = 0; i < ship.length; i++) {
        if (ship.direction == 'H') {
            dest.x = PADDING + (ship.x + i) * CELL_SIZE;
            dest.y = PADDING + ship.y * CELL_SIZE;
        } else {
            dest.x = PADDING + ship.x * CELL_SIZE;
            dest.y = PADDING + (ship.y + i) * CELL_SIZE;
        }
        dest.w = CELL_SIZE;
        dest.h = CELL_SIZE;

        SDL_RenderCopy(renderer, ship_texture, NULL, &dest);
    }
}

// Hiển thị hướng dẫn và trạng thái đặt tàu
void draw_instructions(SDL_Renderer *renderer, TTF_Font *font, char current_direction, int current_length) {
    SDL_Color white = {255, 255, 255, 255};

    // Hướng dẫn
    SDL_Surface *surface = TTF_RenderText_Solid(font, "Press H for Horizontal, V for Vertical", white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {PADDING, PADDING + GRID_SIZE * CELL_SIZE + 20, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Trạng thái đặt tàu
    char direction_text[50];
    snprintf(direction_text, sizeof(direction_text), "Direction: %s", current_direction == 'H' ? "Horizontal" : "Vertical");
    surface = TTF_RenderText_Solid(font, direction_text, white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    dest = (SDL_Rect){PADDING, PADDING + GRID_SIZE * CELL_SIZE + 50, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);

    // Thông tin độ dài tàu
    char length_text[50];
    snprintf(length_text, sizeof(length_text), "Current Ship Length: %d", current_length);
    surface = TTF_RenderText_Solid(font, length_text, white);
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    dest = (SDL_Rect){PADDING, PADDING + GRID_SIZE * CELL_SIZE + 80, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Kiểm tra vị trí hợp lệ
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

// Đặt tàu vào bảng
void place_ship(int board[GRID_SIZE][GRID_SIZE], Ship ship) {
    for (int i = 0; i < ship.length; i++) {
        int x = ship.x + (ship.direction == 'H' ? i : 0);
        int y = ship.y + (ship.direction == 'V' ? i : 0);
        board[y][x] = 1;
    }
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    SDL_Window *window = SDL_CreateWindow("Place Ships", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
    SDL_Texture *ship_texture = IMG_LoadTexture(renderer, "ship.png");

    if (!ship_texture) {
        printf("Failed to load ship texture: %s\n", IMG_GetError());
        return 1;
    }

    int board[GRID_SIZE][GRID_SIZE] = {0};
    Ship ships[4];
    int ship_sizes[4] = {5, 4, 3, 2};
    int current_ship = 0;
    char current_direction = 'H';

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            } else if (e.type == SDL_MOUSEBUTTONDOWN && current_ship < 4) {
                int x = (e.button.x - PADDING) / CELL_SIZE;
                int y = (e.button.y - PADDING) / CELL_SIZE;

                if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
                    Ship ship = {x, y, ship_sizes[current_ship], current_direction};
                    if (is_valid_position(board, ship)) {
                        place_ship(board, ship);
                        ships[current_ship++] = ship;
                    }
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

        draw_grid(renderer, font);

        for (int i = 0; i < current_ship; i++) {
            draw_ship(renderer, ship_texture, ships[i]);
        }

        draw_instructions(renderer, font, current_direction, ship_sizes[current_ship]);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyTexture(ship_texture);
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();

    return 0;
}
