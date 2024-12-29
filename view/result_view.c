#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string.h>
#include <stdbool.h>

// Hàm hiển thị màn hình chiến thắng
void display_victory_screen(SDL_Renderer *renderer, const char *elo) {

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); 
    SDL_RenderClear(renderer);

    // Tải font chữ
    TTF_Font *font = TTF_OpenFont("/home/an/Documents/GitHub/BattelShip/arial.ttf", 48); // Thay đường dẫn font
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    // Tải hình ảnh chiếc cúp
    SDL_Surface *cup_surface = IMG_Load("/home/an/Documents/GitHub/BattelShip/win.jpg"); // Thay đường dẫn hình ảnh
    if (!cup_surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        TTF_CloseFont(font);
        return;
    }
    SDL_Texture *cup_texture = SDL_CreateTextureFromSurface(renderer, cup_surface);
    SDL_FreeSurface(cup_surface);

    // Xác định vị trí chiếc cúp
    SDL_Rect cup_rect;
    cup_rect.w = 400; // Kích thước chiếc cúp
    cup_rect.h = 400;
    cup_rect.x = 800 - cup_rect.w; 
    cup_rect.y = 150;

    // Vẽ chiếc cúp
    SDL_RenderCopy(renderer, cup_texture, NULL, &cup_rect);
    SDL_DestroyTexture(cup_texture);

    // Vẽ chữ "You Win!"
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *win_surface = TTF_RenderText_Solid(font, "You Win!", white);
    SDL_Texture *win_texture = SDL_CreateTextureFromSurface(renderer, win_surface);
    SDL_Rect win_rect = { (1600 - win_surface->w) / 2, 50, win_surface->w, win_surface->h }; // Vị trí chữ
    SDL_FreeSurface(win_surface);
    SDL_RenderCopy(renderer, win_texture, NULL, &win_rect);
    SDL_DestroyTexture(win_texture);

    // Vẽ ELO mới
    char elo_text[256];
    snprintf(elo_text, sizeof(elo_text), "New ELO: %s", elo);
    SDL_Surface *elo_surface = TTF_RenderText_Solid(font, elo_text, white);
    SDL_Texture *elo_texture = SDL_CreateTextureFromSurface(renderer, elo_surface);
    SDL_Rect elo_rect = { (1600 - elo_surface->w) / 2, 400, elo_surface->w, elo_surface->h }; // Vị trí ELO
    SDL_FreeSurface(elo_surface);
    SDL_RenderCopy(renderer, elo_texture, NULL, &elo_rect);
    SDL_DestroyTexture(elo_texture);
    // Tạo nút Confirm
    SDL_Rect confirm_button = { (1600 - 150) / 2, 600, 150, 50 }; // Centered button
    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); // Màu xanh lá
    SDL_RenderFillRect(renderer, &confirm_button);

    // Vẽ chữ "Confirm" trên nút
    SDL_Surface *confirm_surface = TTF_RenderText_Solid(font, "Confirm", white);
    SDL_Texture *confirm_texture = SDL_CreateTextureFromSurface(renderer, confirm_surface);
    SDL_Rect confirm_text_rect = {confirm_button.x + 20, confirm_button.y + 10, confirm_surface->w, confirm_surface->h};
    SDL_FreeSurface(confirm_surface);
    SDL_RenderCopy(renderer, confirm_texture, NULL, &confirm_text_rect);
    SDL_DestroyTexture(confirm_texture);

    // Hiển thị màn hình
    SDL_RenderPresent(renderer);

    // Chờ người dùng bấm nút Confirm
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
                if (x >= confirm_button.x && x <= confirm_button.x + confirm_button.w &&
                    y >= confirm_button.y && y <= confirm_button.y + confirm_button.h) {
                    // Nút Confirm được nhấn
                    running = false;
                }
            }
        }
    }

    // Giải phóng tài nguyên
    TTF_CloseFont(font);
}

//man hinh thua
void display_defeated_screen(SDL_Renderer *renderer, const char *elo) {
    // Clear màn hình
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Màu nền đen
    SDL_RenderClear(renderer);

    // Tải font chữ
    TTF_Font *font = TTF_OpenFont("/home/an/Documents/GitHub/BattelShip/arial.ttf", 48); // Thay đường dẫn font
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    // Tải hình ảnh chiếc cúp
    SDL_Surface *cup_surface = IMG_Load("/home/an/Documents/GitHub/BattelShip/lose.png"); // Thay đường dẫn hình ảnh
    if (!cup_surface) {
        printf("Failed to load image: %s\n", IMG_GetError());
        TTF_CloseFont(font);
        return;
    }
    SDL_Texture *cup_texture = SDL_CreateTextureFromSurface(renderer, cup_surface);
    SDL_FreeSurface(cup_surface);

    // Xác định vị trí chiếc cúp
    SDL_Rect cup_rect;
    cup_rect.w = 400; 
    cup_rect.h = 400;
    cup_rect.x = (1600 - cup_rect.w*2) / 2; 
    cup_rect.y = 150;

    // Vẽ chiếc cúp
    SDL_RenderCopy(renderer, cup_texture, NULL, &cup_rect);
    SDL_DestroyTexture(cup_texture);

    // Vẽ chữ "You Win!"
    SDL_Color white = {255, 255, 255, 255};
    SDL_Surface *win_surface = TTF_RenderText_Solid(font, "You Win!", white);
    SDL_Texture *win_texture = SDL_CreateTextureFromSurface(renderer, win_surface);
    SDL_Rect win_rect = { (1600 - win_surface->w) / 2, 50, win_surface->w, win_surface->h }; // Vị trí chữ
    SDL_FreeSurface(win_surface);
    SDL_RenderCopy(renderer, win_texture, NULL, &win_rect);
    SDL_DestroyTexture(win_texture);

    // Vẽ ELO mới
    char elo_text[256];
    snprintf(elo_text, sizeof(elo_text), "New ELO: %s", elo);
    SDL_Surface *elo_surface = TTF_RenderText_Solid(font, elo_text, white);
    SDL_Texture *elo_texture = SDL_CreateTextureFromSurface(renderer, elo_surface);
    SDL_Rect elo_rect = { (1600 - elo_surface->w) / 2, 400, elo_surface->w, elo_surface->h }; // Vị trí ELO
    SDL_FreeSurface(elo_surface);
    SDL_RenderCopy(renderer, elo_texture, NULL, &elo_rect);
    SDL_DestroyTexture(elo_texture);

    // Tạo nút Confirm
    SDL_Rect confirm_button = { (1600 - 150) / 2, 600, 150, 50 }; // Centered button
    SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255); // Màu xanh lá
    SDL_RenderFillRect(renderer, &confirm_button);

    // Vẽ chữ "Confirm" trên nút
    SDL_Surface *confirm_surface = TTF_RenderText_Solid(font, "Confirm", white);
    SDL_Texture *confirm_texture = SDL_CreateTextureFromSurface(renderer, confirm_surface);
    SDL_Rect confirm_text_rect = {confirm_button.x + 20, confirm_button.y + 10, confirm_surface->w, confirm_surface->h};
    SDL_FreeSurface(confirm_surface);
    SDL_RenderCopy(renderer, confirm_texture, NULL, &confirm_text_rect);
    SDL_DestroyTexture(confirm_texture);

    // Hiển thị màn hình
    SDL_RenderPresent(renderer);

    // Chờ người dùng bấm nút Confirm
    bool running = true;
    SDL_Event event;
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
                break;
            } else if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
                int x = event.button.x;
                int y = event.button.y;
                if (x >= confirm_button.x && x <= confirm_button.x + confirm_button.w &&
                    y >= confirm_button.y && y <= confirm_button.y + confirm_button.h) {
                    // Nút Confirm được nhấn
                    running = false;
                }
            }
        }
    }

    // Giải phóng tài nguyên
    TTF_CloseFont(font);
}