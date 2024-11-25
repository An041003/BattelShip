#include "ui.h"
#include <stdio.h>
#include <string.h>

int ui_init(SDL_Window **window, SDL_Renderer **renderer) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 0;
    }

    if (TTF_Init() < 0) {
        printf("TTF initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 0;
    }

    *window = SDL_CreateWindow("Register/Login", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    return 1;
}

void ui_render_interface(SDL_Renderer *renderer, const char *username, const char *password, const char *message, int option) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);  // Cập nhật đường dẫn phù hợp
    if (!font) {
        printf("Unable to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface;
    SDL_Texture *texture;
    SDL_Rect dest;

    // Hàm hiển thị văn bản
    auto render_text = [&](const char *text, int x, int y) {
        surface = TTF_RenderText_Solid(font, text, color);
        texture = SDL_CreateTextureFromSurface(renderer, surface);
        dest = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &dest);
        SDL_FreeSurface(surface);
        SDL_DestroyTexture(texture);
    };

    // Giao diện chính
    render_text("Register/Login", 250, 20);
    render_text("Username:", 100, 100);
    render_text(username, 300, 100);
    render_text("Password:", 100, 150);
    render_text(password, 300, 150);

    render_text("1: Register", 100, 250);
    render_text("2: Login", 100, 300);

    if (message) {
        render_text(message, 100, 350);
    }

    SDL_RenderPresent(renderer);
    TTF_CloseFont(font);
}

void ui_handle_input(SDL_Event *e, char *username, char *password, int *option, int *quit) {
    if (e->type == SDL_QUIT) {
        *quit = 1;
    } else if (e->type == SDL_KEYDOWN) {
        if (e->key.keysym.sym == SDLK_1) {
            *option = 1;
        } else if (e->key.keysym.sym == SDLK_2) {
            *option = 2;
        } else if (e->key.keysym.sym == SDLK_RETURN) {
            // Nhấn Enter sẽ gửi tín hiệu (xử lý ở file logic)
        }
    } else if (e->type == SDL_TEXTINPUT) {
        // Nhập liệu username hoặc password
        char *target = *option == 1 ? username : password;
        if (strlen(target) < 254) {
            strcat(target, e->text.text);
        }
    } else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_BACKSPACE) {
        char *target = *option == 1 ? username : password;
        if (strlen(target) > 0) {
            target[strlen(target) - 1] = '\0';
        }
    }
}

void ui_cleanup(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}