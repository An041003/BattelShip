#include <stdio.h>
#include <SDL2/SDL.h>
#include "view/base_view.h"
#include "view/login_view.h"
#include "view/register_view.h"
#include "model/network.h"
#include "protocol.h"

int main() {
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!init_view(&window, &renderer)) {
        return -1;
    }

    int sock = connect_to_server("127.0.0.1", PORT);
    if (sock < 0) {
        render_text(renderer, "Failed to connect to server.");
        SDL_Delay(2000);
        cleanup_view(window, renderer);
        return -1;
    }

    render_text(renderer, "1. Register\n2. Login");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        register_view(renderer, sock);
    } else if (choice == 2) {
        login_view(renderer, sock);
    } else {
        render_text(renderer, "Invalid choice. Exiting.");
    }

    close(sock);
    SDL_Delay(3000);
    cleanup_view(window, renderer);
    return 0;
}
