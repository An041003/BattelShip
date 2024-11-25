#include "ui.h"
#include <stdio.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Hiển thị văn bản trên giao diện
void render_text(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y, SDL_Color color) {
    if (!text || strlen(text) == 0) {
        text = " "; // Xử lý khi chuỗi rỗng
    }

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Unable to create text surface: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Unable to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        return;
    }

    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

// Khởi tạo giao diện SDL
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

    *window = SDL_CreateWindow("Register/Login", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    if (!*window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        TTF_Quit();
        SDL_Quit();
        return 0;
    }

    return 1;
}

// Vẽ giao diện chính
void ui_render_interface(SDL_Renderer *renderer, const char *username, const char *password, const char *message, int option) {
    SDL_SetRenderDrawColor(renderer, 4, 5, 15, 255);
    SDL_RenderClear(renderer);

    TTF_Font *font = TTF_OpenFont("/home/an/Documents/GitHub/BattelShip/ui/arial.ttf", 24);  // Cập nhật đường dẫn phù hợp
    if (!font) {
        printf("Unable to load font: %s\n", TTF_GetError());
        return;
    }

    SDL_Color color = {255, 255, 255};

    // Hiển thị nội dung giao diện
    render_text(renderer, font, "Register/Login", 250, 20, color);
    render_text(renderer, font, "Username:", 100, 100, color);
    render_text(renderer, font, username, 300, 100, color);
    render_text(renderer, font, "Password:", 100, 150, color);
    render_text(renderer, font, password, 300, 150, color);

    render_text(renderer, font, "1: Register", 200, 250, color);
    render_text(renderer, font, "2: Login", 200, 300, color);

    SDL_Rect usernameBorder = {300, 100, 300, 35};
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderDrawRect(renderer, &usernameBorder);

    SDL_Rect passwordBorder = {300, 150, 300, 35};
    SDL_SetRenderDrawColor(renderer, 255,255,255,255);
    SDL_RenderDrawRect(renderer, &passwordBorder);

    SDL_Rect LBorder = {200, 250, 200, 40};
    SDL_SetRenderDrawColor(renderer, 0,255,0,255);
    SDL_RenderDrawRect(renderer, &LBorder);

    SDL_Rect RBorder = {200, 300, 200, 40};
    SDL_SetRenderDrawColor(renderer, 0,0,255,255);
    SDL_RenderDrawRect(renderer, &RBorder);


    if (message) {
        render_text(renderer, font, message, 100, 350, color);
    }

    SDL_RenderPresent(renderer);
    TTF_CloseFont(font);
}

// Hàm xử lý nhập liệu
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
        char *target = (*option == 1) ? username : password;
        if (strlen(target) < 254) {
            strcat(target, e->text.text);
        }
    } else if (e->type == SDL_KEYDOWN && e->key.keysym.sym == SDLK_BACKSPACE) {
        char *target = (*option == 1) ? username : password;
        if (strlen(target) > 0) {
            target[strlen(target) - 1] = '\0';
        }
    }
}

// Hàm giải phóng tài nguyên
void ui_cleanup(SDL_Window *window, SDL_Renderer *renderer) {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}
