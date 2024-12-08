#ifndef BASE_VIEW_H
#define BASE_VIEW_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>


// Định nghĩa các màu sắc
typedef struct
{
    SDL_Color white;
    SDL_Color black;
    SDL_Color blue;
    SDL_Color green;
    SDL_Color red;
} ColorScheme;
// Hàm để khởi tạo SDL và TTF
bool init_sdl(const char *window_title, int width, int height, SDL_Window **window, SDL_Renderer **renderer);
bool cleanup_view(SDL_Window **window, SDL_Renderer **renderer);
// Hàm để vẽ nút (Button)
void draw_button(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, const char *text, TTF_Font *font);
// Hàm để hiển thị text
void draw_text(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color, TTF_Font *font);

#endif
