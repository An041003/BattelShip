#ifndef REGISTER_VIEW_H
#define REGISTER_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Hàm để render giao diện đăng ký
void render_register(SDL_Renderer *renderer, TTF_Font *font, char *username, char *password);

#endif
