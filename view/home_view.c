#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

void render_home(SDL_Renderer *renderer, TTF_Font *font, const char *username, int elo) {
    // Xóa màn hình với màu nền
    SDL_SetRenderDrawColor(renderer, 50, 50, 200, 255);
    SDL_RenderClear(renderer);

    // Định nghĩa màu sắc
    SDL_Color white = {255, 255, 255};
    SDL_Color blue = {50, 100, 200};

    // Hiển thị tên người dùng
    char welcome_text[100];
    snprintf(welcome_text, sizeof(welcome_text), "Welcome, %s!", username);
    SDL_Surface *welcome_surface = TTF_RenderText_Solid(font, welcome_text, white);
    SDL_Texture *welcome_texture = SDL_CreateTextureFromSurface(renderer, welcome_surface);
    SDL_Rect welcome_rect = {540, 50, 200, 50}; 
    SDL_RenderCopy(renderer, welcome_texture, NULL, &welcome_rect);

    // Hiển thị điểm Elo
    char elo_text[50];
    snprintf(elo_text, sizeof(elo_text), "Elo: %d", elo);
    SDL_Surface *elo_surface = TTF_RenderText_Solid(font, elo_text, white);
    SDL_Texture *elo_texture = SDL_CreateTextureFromSurface(renderer, elo_surface);
    SDL_Rect elo_rect = {540, 100, 200, 50}; 
    SDL_RenderCopy(renderer, elo_texture, NULL, &elo_rect);

    // Định nghĩa các button
    SDL_Rect find_match_btn = {440, 150, 400, 80};
    SDL_Rect history_btn = {440, 260, 400, 80};
    SDL_Rect logout_btn = {440, 370, 400, 80};

    // Vẽ các button
    SDL_SetRenderDrawColor(renderer, blue.r, blue.g, blue.b, 255);
    SDL_RenderFillRect(renderer, &find_match_btn);
    SDL_RenderFillRect(renderer, &history_btn);
    SDL_RenderFillRect(renderer, &logout_btn);

    // Vẽ chữ trên các button
    SDL_Surface *find_match_surface = TTF_RenderText_Solid(font, "Find Match", white);
    SDL_Texture *find_match_texture = SDL_CreateTextureFromSurface(renderer, find_match_surface);
    SDL_Rect find_match_text_rect = {520, 165, 240, 50}; 
    SDL_RenderCopy(renderer, find_match_texture, NULL, &find_match_text_rect);

    SDL_Surface *history_surface = TTF_RenderText_Solid(font, "View History", white);
    SDL_Texture *history_texture = SDL_CreateTextureFromSurface(renderer, history_surface);
    SDL_Rect history_text_rect = {520, 275, 240, 50}; 
    SDL_RenderCopy(renderer, history_texture, NULL, &history_text_rect);

    SDL_Surface *logout_surface = TTF_RenderText_Solid(font, "Log Out", white);
    SDL_Texture *logout_texture = SDL_CreateTextureFromSurface(renderer, logout_surface);
    SDL_Rect logout_text_rect = {520, 385, 240, 50}; 
    SDL_RenderCopy(renderer, logout_texture, NULL, &logout_text_rect);

    // Giải phóng tài nguyên

    SDL_DestroyTexture(find_match_texture);
    SDL_FreeSurface(find_match_surface);

    SDL_DestroyTexture(history_texture);
    SDL_FreeSurface(history_surface);

    SDL_DestroyTexture(logout_texture);
    SDL_FreeSurface(logout_surface);

    // Hiển thị renderer
    SDL_RenderPresent(renderer);
}


void home_view(SDL_Renderer *renderer) {
    TTF_Font *font = TTF_OpenFont("/home/an/Documents/GitHub/BattelShip/arial.ttf", 24);
    if (!font) {
        printf("Failed to load font: %s\n", TTF_GetError());
        return;
    }

    bool home_running = true;
    SDL_Event event;

    while (home_running) {
        // Vẽ giao diện trang chủ
        char username[50];
        int elo;
        render_home(renderer, font, username, elo);

        // Xử lý sự kiện
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                home_running = false; // Thoát chương trình
            }

            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= 440 && x <= 840 && y >= 150 && y <= 230) {
                    printf("Find Match button clicked\n");
                    // find_match_view(renderer);
                } else if (x >= 440 && x <= 840 && y >= 260 && y <= 340) {
                    printf("View History button clicked\n");
                    // history_view(renderer);
                } else if (x >= 440 && x <= 840 && y >= 370 && y <= 450) {
                    printf("Logout button clicked\n");
                    home_running = false; 
                }
            }
        }
    }

    TTF_CloseFont(font);
}

