#ifndef HOME_VIEW_H
#define HOME_VIEW_H

#include <SDL2/SDL.h>

// Hàm hiển thị giao diện trang chủ
void render_home(SDL_Renderer *renderer, TTF_Font *font, const char *username, int elo);
void home_view(SDL_Renderer *renderer, int sock);

#endif // HOME_VIEW_H
