#ifndef LOGIN_VIEW_H
#define LOGIN_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

// Hàm render giao diện đăng nhập
void render_login(SDL_Renderer *renderer, TTF_Font *font, char *username, char *password, bool input_username);

void login_view(SDL_Renderer *renderer, int sock);

#endif
