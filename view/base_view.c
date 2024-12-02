#include "base_view.h"
#include <stdio.h>

// Hàm để khởi tạo SDL và TTF
bool init_sdl(const char *window_title, int width, int height, SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    if (TTF_Init() == -1)
    {
        printf("TTF_Init: %s\n", TTF_GetError());
        SDL_Quit();
        return false;
    }

    *window = SDL_CreateWindow(window_title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (*window == NULL)
    {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (*renderer == NULL)
    {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(*window);
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    return true;
}

// Hàm để vẽ nút (Button)
void draw_button(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, const char *text, TTF_Font *font)
{
    // Vẽ nền của nút
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);

    // Vẽ viền nút
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu trắng cho viền
    SDL_RenderDrawRect(renderer, &rect);

    // Hiển thị text
    draw_text(renderer, text, rect.x + 10, rect.y + 10, (SDL_Color){255, 255, 255, 255}, font); // Màu trắng cho text
}

// Hàm để hiển thị text
void draw_text(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color, TTF_Font *font)
{
    SDL_Surface *text_surface = TTF_RenderText_Solid(font, text, color);
    if (!text_surface)
    {
        printf("TTF_RenderText_Solid Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture *text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
    if (!text_texture)
    {
        printf("SDL_CreateTextureFromSurface Error: %s\n", SDL_GetError());
        SDL_FreeSurface(text_surface);
        return;
    }

    SDL_Rect text_rect = {x, y, text_surface->w, text_surface->h};
    SDL_RenderCopy(renderer, text_texture, NULL, &text_rect);

    SDL_FreeSurface(text_surface);
    SDL_DestroyTexture(text_texture);
}
