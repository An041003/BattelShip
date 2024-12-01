#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define GRID_SIZE 10
#define CELL_SIZE 40
#define PADDING 20

// Hàm vẽ lưới
void draw_grid(SDL_Renderer *renderer, int start_x, int start_y, int grid_size, int cell_size) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 255); // Màu lưới
    for (int i = 0; i <= grid_size; i++) {
        // Vẽ đường ngang
        SDL_RenderDrawLine(renderer, start_x, start_y + i * cell_size, start_x + grid_size * cell_size, start_y + i * cell_size);
        // Vẽ đường dọc
        SDL_RenderDrawLine(renderer, start_x + i * cell_size, start_y, start_x + i * cell_size, start_y + grid_size * cell_size);
    }
}

// Hàm vẽ chữ
void render_text(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color, TTF_Font *font) {
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Unable to render text: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest_rect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest_rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("Failed to initialize SDL_ttf: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Battleship", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    TTF_Font *font = TTF_OpenFont("arial.ttf", 16);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool quit = false;
    SDL_Event event;

    while (!quit) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu nền đen
        SDL_RenderClear(renderer);

        // Vẽ lưới Computer
        draw_grid(renderer, PADDING, PADDING, GRID_SIZE, CELL_SIZE);

        // Vẽ lưới Player
        draw_grid(renderer, WINDOW_WIDTH / 2 + PADDING, PADDING, GRID_SIZE, CELL_SIZE);

        // Hiển thị tiêu đề
        SDL_Color white = {255, 255, 255, 255};
        render_text(renderer, "Computer", PADDING + GRID_SIZE * CELL_SIZE / 2 - 30, PADDING - 30, white, font);
        render_text(renderer, "Player", WINDOW_WIDTH / 2 + PADDING + GRID_SIZE * CELL_SIZE / 2 - 30, PADDING - 30, white, font);

        // Hiển thị trạng thái
        render_text(renderer, "Computer: 3", PADDING, PADDING + GRID_SIZE * CELL_SIZE + 10, white, font);
        render_text(renderer, "Player: 2", WINDOW_WIDTH / 2 + PADDING, PADDING + GRID_SIZE * CELL_SIZE + 10, white, font);

        SDL_RenderPresent(renderer);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
