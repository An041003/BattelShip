#include "base_view.h"
#include <stdio.h>
#include <stdbool.h>
#include "../model/board.h"

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
bool cleanup_view(SDL_Window **window, SDL_Renderer **renderer){
    if (renderer) {
        SDL_DestroyRenderer(*renderer);
    }
    if (window) {
        SDL_DestroyWindow(*window);
    }
    SDL_Quit();
    TTF_Quit();
    return true;
};
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

void draw_grid(SDL_Renderer *renderer, TTF_Font *font, int offset_x, int offset_y) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Màu xanh dương cho lưới

    for (int i = 0; i <= GRID_SIZE; i++) {
        // Đường ngang
        SDL_RenderDrawLine(renderer, offset_x, offset_y + i * CELL_SIZE, offset_x + GRID_SIZE * CELL_SIZE, offset_y + i * CELL_SIZE);
        // Đường dọc
        SDL_RenderDrawLine(renderer, offset_x + i * CELL_SIZE, offset_y, offset_x + i * CELL_SIZE, offset_y + GRID_SIZE * CELL_SIZE);
    }

    // Vẽ ký hiệu cột (1-10) và hàng (A-J)
    SDL_Color white = {255, 255, 255, 255};
    for (int i = 0; i < GRID_SIZE; i++) {
        char label[2];

        // Số cột (1-10)
        snprintf(label, sizeof(label), "%d", i + 1);
        draw_text(renderer, label, offset_x + i * CELL_SIZE + CELL_SIZE / 2 - 10, offset_y - 30, white, font);

        // Ký tự hàng (A-J)
        snprintf(label, sizeof(label), "%c", 'A' + i);
        draw_text(renderer, label, offset_x - 30, offset_y + i * CELL_SIZE + CELL_SIZE / 2 - 10, white, font);
    }
}


void draw_ship(SDL_Renderer *renderer, SDL_Texture *ship_texture, Ship ship) {
    SDL_Rect dest;

    for (int i = 0; i < ship.length; i++) {
        if (ship.direction == 'H') {
            dest.x = PADDING + (ship.x + i) * CELL_SIZE;
            dest.y = PADDING + ship.y * CELL_SIZE;
        } else {
            dest.x = PADDING + ship.x * CELL_SIZE;
            dest.y = PADDING + (ship.y + i) * CELL_SIZE;
        }
        dest.w = CELL_SIZE;
        dest.h = CELL_SIZE;

        SDL_RenderCopy(renderer, ship_texture, NULL, &dest);
    }
}



void draw_instructions(SDL_Renderer *renderer, TTF_Font *font, char current_direction, int current_length) {

    SDL_Color white = {255, 255, 255, 255};



    // Hướng dẫn

    SDL_Surface *surface = TTF_RenderText_Solid(font, "Press H for Horizontal, V for Vertical", white);

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dest = {PADDING, PADDING + GRID_SIZE * CELL_SIZE + 20, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);

    SDL_DestroyTexture(texture);



    // Trạng thái đặt tàu

    char direction_text[50];

    snprintf(direction_text, sizeof(direction_text), "Direction: %s", current_direction == 'H' ? "Horizontal" : "Vertical");

    surface = TTF_RenderText_Solid(font, direction_text, white);

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    dest = (SDL_Rect){PADDING, PADDING + GRID_SIZE * CELL_SIZE + 50, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);

    SDL_DestroyTexture(texture);



    // Thông tin độ dài tàu

    char length_text[50];

    snprintf(length_text, sizeof(length_text), "Current Ship Length: %d", current_length);

    surface = TTF_RenderText_Solid(font, length_text, white);

    texture = SDL_CreateTextureFromSurface(renderer, surface);

    dest = (SDL_Rect){PADDING, PADDING + GRID_SIZE * CELL_SIZE + 80, surface->w, surface->h};

    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);

    SDL_DestroyTexture(texture);

}
