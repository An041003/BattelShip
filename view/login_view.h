#ifndef LOGIN_VIEW_H
#define LOGIN_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Hàm để render giao diện đăng nhập
void render_login(SDL_Renderer *renderer, TTF_Font *font, char *username, char *password);

#endif
