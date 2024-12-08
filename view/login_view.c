#include "login_view.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdbool.h>
#include "../protocol.h"
#include "../model/network.h"
#include "base_view.h"

void render_login(SDL_Renderer *renderer, TTF_Font *font, char *username, char *password, bool input_username) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255};
    SDL_Color black = {0, 0, 0};
    SDL_Color blue = {50, 100, 200};

    // Các hộp hiển thị
    SDL_Rect username_box = {300, 150, 400, 50};
    SDL_Rect password_box = {300, 250, 400, 50};
    SDL_Rect login_button = {300, 350, 200, 50};

    // Vẽ tiêu đề
    SDL_Surface *title_surface = TTF_RenderText_Solid(font, "User Login", white);
    SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    SDL_Rect title_rect = {300, 50, 200, 50};
    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);

    // Vẽ hộp nhập username
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    SDL_RenderDrawRect(renderer, &username_box);

    SDL_Surface *username_label_surface = TTF_RenderText_Solid(font, "Username:", white);
    SDL_Texture *username_label_texture = SDL_CreateTextureFromSurface(renderer, username_label_surface);
    SDL_Rect username_label_rect = {150, 150, 100, 50};
    SDL_RenderCopy(renderer, username_label_texture, NULL, &username_label_rect);

    // Hiển thị nội dung đã nhập của username
    int username_width = 0, username_height = 0;
    TTF_SizeText(font, username, &username_width, &username_height);

    char truncated_username[256];
    if (username_width > username_box.w - 20) {
        snprintf(truncated_username, sizeof(truncated_username), "%s", &username[strlen(username) - (username_box.w - 20) / 10]);
    } else {
        strcpy(truncated_username, username);
    }

    SDL_Surface *username_content_surface = TTF_RenderText_Solid(font, truncated_username, white);
    SDL_Texture *username_content_texture = SDL_CreateTextureFromSurface(renderer, username_content_surface);
    SDL_Rect username_content_rect = {username_box.x + 5, username_box.y + 10, username_box.w - 10, username_height};
    SDL_RenderCopy(renderer, username_content_texture, NULL, &username_content_rect);

    // Vẽ hộp nhập password
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    SDL_RenderDrawRect(renderer, &password_box);

    SDL_Surface *password_label_surface = TTF_RenderText_Solid(font, "Password:", white);
    SDL_Texture *password_label_texture = SDL_CreateTextureFromSurface(renderer, password_label_surface);
    SDL_Rect password_label_rect = {150, 250, 100, 50};
    SDL_RenderCopy(renderer, password_label_texture, NULL, &password_label_rect);

    // Hiển thị nội dung đã nhập của password
    int password_width = 0, password_height = 0;
    TTF_SizeText(font, password, &password_width, &password_height);

    char truncated_password[256];
    if (password_width > password_box.w - 20) {
        snprintf(truncated_password, sizeof(truncated_password), "%s", &password[strlen(password) - (password_box.w - 20) / 10]);
    } else {
        strcpy(truncated_password, password);
    }

    SDL_Surface *password_content_surface = TTF_RenderText_Solid(font, truncated_password, white);
    SDL_Texture *password_content_texture = SDL_CreateTextureFromSurface(renderer, password_content_surface);
    SDL_Rect password_content_rect = {password_box.x + 5, password_box.y + 10, password_box.w - 10, password_height};
    SDL_RenderCopy(renderer, password_content_texture, NULL, &password_content_rect);

    // Vẽ nút đăng nhập
    SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, 255);
    SDL_RenderFillRect(renderer, &login_button);

    SDL_Surface *login_surface = TTF_RenderText_Solid(font, "Login", white);
    SDL_Texture *login_texture = SDL_CreateTextureFromSurface(renderer, login_surface);
    SDL_Rect login_rect = {350, 360, 100, 30};
    SDL_RenderCopy(renderer, login_texture, NULL, &login_rect);

    // Vẽ con trỏ nhấp nháy
    if (SDL_GetTicks() / 500 % 2 == 0) {
        SDL_Rect cursor_rect;
        if (input_username) {
            cursor_rect.x = username_box.x + 5 + username_width;
            cursor_rect.y = username_box.y + 10;
            cursor_rect.w = 2;
            cursor_rect.h = username_height;
        } else {
            cursor_rect.x = password_box.x + 5 + password_width;
            cursor_rect.y = password_box.y + 10;
            cursor_rect.w = 2;
            cursor_rect.h = password_height;
        }
        SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
        SDL_RenderFillRect(renderer, &cursor_rect);
    }

    // Giải phóng tài nguyên
    SDL_DestroyTexture(title_texture);
    SDL_FreeSurface(title_surface);

    SDL_DestroyTexture(username_label_texture);
    SDL_FreeSurface(username_label_surface);

    SDL_DestroyTexture(username_content_texture);
    SDL_FreeSurface(username_content_surface);

    SDL_DestroyTexture(password_label_texture);
    SDL_FreeSurface(password_label_surface);

    SDL_DestroyTexture(password_content_texture);
    SDL_FreeSurface(password_content_surface);

    SDL_DestroyTexture(login_texture);
    SDL_FreeSurface(login_surface);

    SDL_RenderPresent(renderer);
}

void login_view(SDL_Renderer *renderer, int sock) {
    TTF_Font *font = TTF_OpenFont("/home/an/Documents/GitHub/BattelShip/arial.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    char username[256] = {0};
    char password[256] = {0};
    bool login_running = true;
    bool input_username = true;

    while (login_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                login_running = false;
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_TAB) {
                    input_username = !input_username;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (input_username && strlen(username) > 0) {
                        username[strlen(username) - 1] = '\0';
                    } else if (!input_username && strlen(password) > 0) {
                        password[strlen(password) - 1] = '\0';
                    }
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    // Handle sending login credentials here
                    login_running = false;
                } else if (event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z) {
                    char ch = (char)event.key.keysym.sym;
                    if (input_username && strlen(username) < 255) {
                        strncat(username, &ch, 1);
                    } else if (!input_username && strlen(password) < 255) {
                        strncat(password, &ch, 1);
                    }
                }
            }
        }

        render_login(renderer, font, username, password, input_username);
    }

    TTF_CloseFont(font);
}
