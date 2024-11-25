#ifndef UI_H
#define UI_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// Khởi tạo SDL và tạo cửa sổ, renderer
int ui_init(SDL_Window **window, SDL_Renderer **renderer);

// Hiển thị giao diện chính
void ui_render_interface(SDL_Renderer *renderer, const char *username, const char *password, const char *message, int option);

// Xử lý sự kiện đầu vào
void ui_handle_input(SDL_Event *e, char *username, char *password, int *option, int *quit);

// Dọn dẹp tài nguyên SDL
void ui_cleanup(SDL_Window *window, SDL_Renderer *renderer);

#endif