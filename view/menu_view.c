#include "menu_view.h"
#include "base_view.h"
#include <SDL2/SDL.h>
#include <stdbool.h>

// Hàm để hiển thị menu và xử lý sự kiện
int render_menu_view(SDL_Renderer *renderer)
{
    TTF_Font *font = TTF_OpenFont("/home/hoangmanhkien/Lập Trình Mạng/BattelShip/arial.ttf",24);
    if(!font){
        printf("Unload font %s\n", TTF_GetError());
        return;
    }
    bool waiting = true;
    SDL_Event event;
    int choice = -1;

    while (waiting)
    {
        
        SDL_SetRenderDrawColor(renderer, 0, 0, 51, 255);
        SDL_RenderClear(renderer);

        draw_text(renderer, "Welcome! Choose an option:", 200, 100,(SDL_Color){255, 255, 255, 255}, font);
        SDL_Rect login_button = {200, 200, 200, 50};
        SDL_Rect register_button = {200, 300, 200, 50};

        draw_button(renderer, login_button, (SDL_Color){0, 122, 255, 255}, "1. Login", font);
        draw_button(renderer, register_button, (SDL_Color){0, 200, 0, 255}, "2. Register", font);

        SDL_RenderPresent(renderer); 

        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                waiting = false;
                choice = -1;
                break;
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                int x = event.button.x;
                int y = event.button.y;

                if (x >= login_button.x && x <= login_button.x + login_button.w &&
                    y >= login_button.y && y <= login_button.y + login_button.h)
                {
                    choice = 2; 
                    waiting = false;
                }

                if (x >= register_button.x && x <= register_button.x + register_button.w &&
                    y >= register_button.y && y <= register_button.y + register_button.h)
                {
                    choice = 1; 
                    waiting = false;
                }
            }
        }
    }
    return choice;
}
void on_fire_button_click(SDL_Renderer *renderer, int x, int y, int board[10][10], int sock) {
    send_fire(x, y, sock); // Gửi tọa độ bắn đến server

    // Render lại bảng chơi
    render_game_board(renderer, board, 50); // cellSize = 50
    SDL_RenderPresent(renderer);
}

