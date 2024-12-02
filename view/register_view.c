#include "register_view.h"
#include "../model/network.h"
#include "../protocol.h"
#include "base_view.h"
#include <string.h>

void register_view(SDL_Renderer *renderer, int sock) {
    char username[50], password[50], buffer[BUFFER_SIZE];

    render_text(renderer, "Enter your username:");
    scanf("%s", username);

    render_text(renderer, "Enter your password:");
    scanf("%s", password);

    // Gửi lệnh REGISTER và dữ liệu tới server
    send_to_server(sock, REGISTER);
    send_to_server(sock, username);
    send_to_server(sock, password);

    // Nhận phản hồi từ server
    receive_from_server(sock, buffer, sizeof(buffer));
    render_text(renderer, buffer);
}
