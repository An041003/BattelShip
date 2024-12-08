#include "register_view.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "../protocol.h"
#include "../model/network.h"
#include <stdbool.h>
void render_register(SDL_Renderer *renderer, TTF_Font *font, char *username, char *password)
{
    SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255};
    SDL_Color blue = {50, 100, 200};

    SDL_Rect username_box = {200, 150, 200, 50};
    SDL_Rect password_box = {200, 250, 200, 50};
    SDL_Rect register_button = {200, 350, 200, 50};

    SDL_Surface *title_surface = TTF_RenderText_Solid(font, "User register", white);
    SDL_Texture *title_texture = SDL_CreateTextureFromSurface(renderer, title_surface);
    SDL_Rect title_rect = {200, 50, 200, 50};

    SDL_Surface *username_surface = TTF_RenderText_Solid(font, "Username:", white);
    SDL_Texture *username_texture = SDL_CreateTextureFromSurface(renderer, username_surface);

    SDL_Surface *password_surface = TTF_RenderText_Solid(font, "Password:", white);
    SDL_Texture *password_texture = SDL_CreateTextureFromSurface(renderer, password_surface);

    SDL_Surface *register_surface = TTF_RenderText_Solid(font, "Register", white);
    SDL_Texture *register_texture = SDL_CreateTextureFromSurface(renderer, register_surface);

    SDL_RenderCopy(renderer, title_texture, NULL, &title_rect);

    SDL_RenderCopy(renderer, username_texture, NULL, &username_box);
    SDL_RenderCopy(renderer, password_texture, NULL, &password_box);

    SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, 255);
    SDL_RenderFillRect(renderer, &register_button);
    SDL_RenderCopy(renderer, register_texture, NULL, &register_button);

    SDL_DestroyTexture(title_texture);
    SDL_DestroyTexture(username_texture);
    SDL_DestroyTexture(password_texture);
    SDL_DestroyTexture(register_texture);
    SDL_FreeSurface(title_surface);
    SDL_FreeSurface(username_surface);
    SDL_FreeSurface(password_surface);
    SDL_FreeSurface(register_surface);

    SDL_RenderPresent(renderer);
}
void register_view(SDL_Renderer *renderer, int sock) {
    TTF_Font *font = TTF_OpenFont("/home/an/Documents/GitHub/BattelShip/arial.ttf", 24); 
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    char username[256] = {0}; // Tên người dùng
    char password[256] = {0}; // Mật khẩu
    bool register_running = true;
    bool input_username = true; // Trạng thái: nhập tên người dùng hay mật khẩu
    int cursor_pos = 0;         // Vị trí con trỏ nhập liệu

    while (register_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                register_running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_TAB) {
                    // Chuyển đổi giữa nhập tên và mật khẩu
                    input_username = !input_username;
                    cursor_pos = 0;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    // Xóa ký tự
                    if (cursor_pos > 0) {
                        if (input_username) {
                            username[--cursor_pos] = '\0';
                        } else {
                            password[--cursor_pos] = '\0';
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    // Gửi dữ liệu đăng nhập khi nhấn Enter
                    if (strlen(username) > 0 && strlen(password) > 0) {
                        char register_request[512];
                        snprintf(register_request, sizeof(register_request), "register %s %s", username, password);
                        send(sock, register_request, strlen(register_request), 0);

                        // Nhận phản hồi từ máy chủ
                        char response[256];
                        int bytes_received = recv(sock, response, sizeof(response) - 1, 0);
                        response[bytes_received] = '\0';

                        if (strcmp(response, "register_SUCCESS") == 0) {
                            printf("register successful!\n");
                            register_running = false;
                        } else {
                            printf("register failed: %s\n", response);
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    register_running = false; // Thoát giao diện đăng nhập
                } else if (event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z) {
                    // Nhập ký tự
                    if (cursor_pos < 255) {
                        if (input_username) {
                            username[cursor_pos++] = (char)event.key.keysym.sym;
                            username[cursor_pos] = '\0';
                        } else {
                            password[cursor_pos++] = (char)event.key.keysym.sym;
                            password[cursor_pos] = '\0';
                        }
                    }
                }
            }
        }

        // Hiển thị giao diện đăng nhập
        render_register(renderer, font, username, password);
    }

    TTF_CloseFont(font);
}