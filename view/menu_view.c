#include "menu_view.h"
#include "base_view.h"
#include <SDL2/SDL.h>

// Hàm để hiển thị menu và xử lý sự kiện
int render_menu_view(SDL_Renderer *renderer)
{
    bool waiting = true;
    SDL_Event event;
    int choice = -1;

    while (waiting)
    {
        // Xóa renderer
        SDL_SetRenderDrawColor(renderer, 0, 0, 51, 255); // Màu nền xanh đậm
        SDL_RenderClear(renderer);

        // Hiển thị các nút trong menu
        render_text(renderer, "Welcome! Choose an option:", 200, 100);
        SDL_Rect login_button = {200, 200, 200, 50};
        SDL_Rect register_button = {200, 300, 200, 50};

        draw_button(renderer, login_button, (SDL_Color){0, 122, 255, 255}, "1. Login");
        draw_button(renderer, register_button, (SDL_Color){0, 200, 0, 255}, "2. Register");

        SDL_RenderPresent(renderer); // Vẽ lên màn hình

        // Vòng lặp sự kiện
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

                // Kiểm tra nếu người dùng nhấn vào nút Login
                if (x >= login_button.x && x <= login_button.x + login_button.w &&
                    y >= login_button.y && y <= login_button.y + login_button.h)
                {
                    choice = 2; // Chọn đăng nhập
                    waiting = false;
                }

                // Kiểm tra nếu người dùng nhấn vào nút Register
                if (x >= register_button.x && x <= register_button.x + register_button.w &&
                    y >= register_button.y && y <= register_button.y + register_button.h)
                {
                    choice = 1; // Chọn đăng ký
                    waiting = false;
                }
            }
        }
    }
    return choice;
}
