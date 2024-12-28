#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "view/base_view.h"
#include "view/login_view.h"
#include "view/register_view.h"
#include "view/menu_view.h"
#include "model/network.h"
#include "protocol.h"

// Hàm chính
int main()
{
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    // Khởi tạo SDL và cửa sổ
    if (!init_sdl("",1600,900,&window, &renderer))
    {
        return -1;
    }

    // Kết nối đến server
    int sock = connect_to_server("127.0.0.1", PORT);
    if (sock < 0)
    {
        SDL_Delay(2000);
        cleanup_view(window, renderer);
        return -1;
    }

    bool running = true; // Vòng lặp chính
    while (running)
    {
        // Hiển thị menu
        int choice = render_menu_view(renderer);

        // Xử lý lựa chọn
        switch (choice)
        {
        case 1: // Đăng ký
            register_view(renderer, sock);
            break;
        case 2: // Đăng nhập
            login_view(renderer, sock);
            break;
        case -1: // Thoát
            running = false;
            break;
        default: 
            SDL_Delay(1000);
            break;
        }
    }

    // Đóng kết nối và dọn dẹp
    close(sock);
    cleanup_view(window, renderer);
    return 0;
}
