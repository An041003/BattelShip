#include "base_view.h"
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

int init_view(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization failed: %s\n", SDL_GetError());
        return 0;
    }

    *window = SDL_CreateWindow("Game Client", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() < 0) {
        printf("TTF Initialization failed: %s\n", TTF_GetError());
        return 0;
    }

    return 1;
}

void render_text(SDL_Renderer *renderer, const char *text) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Đây là nơi bạn dùng TTF_Font để vẽ text. Tạm thời chỉ là mô phỏng:
    printf("%s\n", text);

    SDL_RenderPresent(renderer);
}

void cleanup_view(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
