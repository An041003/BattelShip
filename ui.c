#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Hàm hiển thị văn bản
void renderText(SDL_Renderer *renderer, const char *text, int x, int y, SDL_Color color, TTF_Font *font)
{
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect textRect = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &textRect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

int main()
{
    // Khởi tạo SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    // Khởi tạo SDL_ttf
    if (TTF_Init() == -1)
    {
        printf("TTF_Init Error: %s\n", TTF_GetError());
        return 1;
    }

    // Tạo cửa sổ
    SDL_Window *window = SDL_CreateWindow("Login/Register", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    if (!window)
    {
        printf("SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Tạo renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Tạo font
    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
    if (!font)
    {
        printf("TTF_OpenFont Error: %s\n", TTF_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;
    SDL_Color white = {255, 255, 255};

    while (running)
    {
        // Xử lý sự kiện
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = false;
            }
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                // Kiểm tra nếu nhấp vào nút "Login"
                if (x >= 300 && x <= 500 && y >= 250 && y <= 300)
                {
                    printf("Login button clicked\n");
                }

                // Kiểm tra nếu nhấp vào nút "Register"
                if (x >= 300 && x <= 500 && y >= 350 && y <= 400)
                {
                    printf("Register button clicked\n");
                }
            }
        }

        // Vẽ màn hình
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Vẽ nút "Login"
        SDL_Rect loginRect = {300, 250, 200, 50};
        SDL_SetRenderDrawColor(renderer, 0, 128, 0, 255);
        SDL_RenderFillRect(renderer, &loginRect);
        renderText(renderer, "Login", 370, 265, white, font);

        // Vẽ nút "Register"
        SDL_Rect registerRect = {300, 350, 200, 50};
        SDL_SetRenderDrawColor(renderer, 0, 0, 128, 255);
        SDL_RenderFillRect(renderer, &registerRect);
        renderText(renderer, "Register", 350, 365, white, font);

        SDL_RenderPresent(renderer);
    }

    // Giải phóng tài nguyên
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
