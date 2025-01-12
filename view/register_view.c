#include "register_view.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include "../protocol.h"
#include "../model/network.h"
#include <stdbool.h>
#include "login_view.h"
void render_register(SDL_Renderer *renderer, TTF_Font *font, char *username, char *password, bool input_username) {
    SDL_SetRenderDrawColor(renderer, 20, 20, 50, 255);
    SDL_RenderClear(renderer);

    SDL_Color white = {255, 255, 255};
    SDL_Color black = {0, 0, 0};
    SDL_Color blue = {50, 100, 200};

    // Các hộp hiển thị
    SDL_Rect username_box = {300, 150, 600, 500};
    SDL_Rect password_box = {300, 250, 400, 50};
    SDL_Rect register_button = {300, 350, 200, 50};

    // Vẽ tiêu đề
    SDL_Surface *title_surface = TTF_RenderText_Solid(font, "User Register", white);
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

    char truncated_username[256];
    int max_width = username_box.w - 20; 
    int username_width = 0, username_height = 0;
    TTF_SizeText(font, username, &username_width, &username_height);

    if (username_width > max_width) {
        int chars_to_show = max_width / 10; 
        snprintf(truncated_username, sizeof(truncated_username), "...%s",
                 &username[strlen(username) - chars_to_show]);
    } else {
        strcpy(truncated_username, username);
    }

    SDL_Surface *username_content_surface = TTF_RenderText_Solid(font, truncated_username, white);
    SDL_Texture *username_content_texture = SDL_CreateTextureFromSurface(renderer, username_content_surface);
    SDL_Rect username_content_rect = {username_box.x + 5, username_box.y + (username_box.h - username_height) / 2,
                                       username_width, username_height};
    SDL_RenderCopy(renderer, username_content_texture, NULL, &username_content_rect);

    // Vẽ hộp nhập password
    SDL_SetRenderDrawColor(renderer, white.r, white.g, white.b, 255);
    SDL_RenderDrawRect(renderer, &password_box);

    SDL_Surface *password_label_surface = TTF_RenderText_Solid(font, "Password:", white);
    SDL_Texture *password_label_texture = SDL_CreateTextureFromSurface(renderer, password_label_surface);
    SDL_Rect password_label_rect = {150, 250, 100, 50};
    SDL_RenderCopy(renderer, password_label_texture, NULL, &password_label_rect);

    char masked_password[256] = {0};
    for (size_t i = 0; i < strlen(password); i++) {
        strncat(masked_password, "*", 1);
    }

    char truncated_password[256];
    int password_width = 0, password_height = 0;
    TTF_SizeText(font, masked_password, &password_width, &password_height);

    if (password_width > max_width) {
        int chars_to_show = max_width / 10;
        snprintf(truncated_password, sizeof(truncated_password), "...%s",
                 &masked_password[strlen(masked_password) - chars_to_show]);
    } else {
        strcpy(truncated_password, masked_password);
    }

    SDL_Surface *password_content_surface = TTF_RenderText_Solid(font, truncated_password, white);
    SDL_Texture *password_content_texture = SDL_CreateTextureFromSurface(renderer, password_content_surface);
    SDL_Rect password_content_rect = {password_box.x + 5, password_box.y + (password_box.h - password_height) / 2,
                                       password_width, password_height};
    SDL_RenderCopy(renderer, password_content_texture, NULL, &password_content_rect);

    SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, 255);
    SDL_RenderFillRect(renderer, &register_button);

    SDL_Surface *register_surface = TTF_RenderText_Solid(font, "Register", white);
    SDL_Texture *register_texture = SDL_CreateTextureFromSurface(renderer, register_surface);
    SDL_Rect register_rect = {350, 360, 100, 30};
    SDL_RenderCopy(renderer, register_texture, NULL, &register_rect);

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

    SDL_DestroyTexture(register_texture);
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
    bool input_username = true; 
    int cursor_pos = 0;         

    while (register_running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                register_running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_TAB) {
                    input_username = !input_username;
                    cursor_pos = 0;
                } else if (event.key.keysym.sym == SDLK_BACKSPACE) {
                    if (cursor_pos > 0) {
                        if (input_username) {
                            username[--cursor_pos] = '\0';
                        } else {
                            password[--cursor_pos] = '\0';
                        }
                    }
                } else if (event.key.keysym.sym == SDLK_RETURN) {
                    
                    if (strlen(username) > 0 && strlen(password) > 0) {
                        char register_request[512];
                        snprintf(register_request, sizeof(register_request), "REGISTER %s %s", username, password);
                        send(sock, register_request, strlen(register_request), 0);
                        printf("Sending to server: %s\n", register_request);

                        
                        char response[256];
                        memset(response, 0, sizeof(response)); 
                        int bytes_received = recv(sock, response, sizeof(response) - 1, 0);

                        if (bytes_received > 0) {
                            response[bytes_received] = '\0'; 

                        if (strcmp(response, "REGISTER") == 0) {
                            printf("Register successful!\n");
                            login_view(renderer,sock);
                             register_running = false;
                         } else {
                               printf("Register failed: %s\n", response);
                                 }
                            } else {
                                 printf("Error receiving response from server.\n");
                            }                       

                    }
                } else if (event.key.keysym.sym == SDLK_ESCAPE) {
                    register_running = false; 
                } else if (event.key.keysym.sym >= SDLK_SPACE && event.key.keysym.sym <= SDLK_z) {
                    
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

        render_register(renderer, font, username, password, input_username);
    }

    TTF_CloseFont(font);
}