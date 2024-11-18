#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

MYSQL *conn;

void connect_db() {
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "localhost", "root", "your_new_password", "user_db", 0, NULL, 0)) {
        printf("Kết nối thất bại: %s\n", mysql_error(conn));
    } else {
        printf("Kết nối thành công!\n");
    }
}

int register_user(const char *username, const char *password) {
    char query[1024];
    sprintf(query, "INSERT INTO users (username, password) VALUES ('%s', '%s')", username, password);

    if (mysql_query(conn, query)) {
        printf("Đăng ký thất bại: %s\n", mysql_error(conn));
        return 0; // Đăng ký thất bại
    }
    return 1; // Đăng ký thành công
}

int login_user(const char *username, const char *password) {
    char query[1024];
    MYSQL_RES *res;
    MYSQL_ROW row;

    sprintf(query, "SELECT * FROM users WHERE username='%s' AND password='%s'", username, password);
    if (mysql_query(conn, query)) {
        printf("Đăng nhập thất bại: %s\n", mysql_error(conn));
        return 0;
    }

    res = mysql_store_result(conn);
    if ((row = mysql_fetch_row(res)) != NULL) {
        mysql_free_result(res);
        return 1; // Đăng nhập thành công
    }
    mysql_free_result(res);
    return 0; // Đăng nhập thất bại
}

void show_message(SDL_Renderer *renderer, const char *message) {
    TTF_Font *font = TTF_OpenFont("/Users/manhkien/Documents/Lập trình mạng/BattelShip/arial.ttf", 24);
    if (!font) {
        printf("Không thể mở font: %s\n", TTF_GetError());
        return;
    }
    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, message, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dest = {100, 100, surface->w, surface->h};
    SDL_RenderClear(renderer);  // Xóa màn hình trước khi vẽ
    SDL_RenderCopy(renderer, texture, NULL, &dest);
    SDL_RenderPresent(renderer);  // Cập nhật màn hình

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void handle_register(SDL_Renderer *renderer, const char *username, const char *password) {
    if (register_user(username, password)) {
        show_message(renderer, "Đăng ký thành công!");
    } else {
        show_message(renderer, "Tên người dùng đã tồn tại!");
    }
}

void handle_input_number(SDL_Renderer *renderer) {
    SDL_Event e;
    char number_str[256] = "";
    int quit = 0;

    show_message(renderer, "nhap 1 so bt ky:");

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
                break;
            }
            if (e.type == SDL_TEXTINPUT) {
                strcat(number_str, e.text.text);
                show_message(renderer, number_str);
            }
            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                quit = 1;
                break;
            }
        }
        SDL_Delay(100); // Tránh tiêu tốn CPU quá mức
    }

    char message[300];
    sprintf(message, "so da nhap: %s", number_str);
    show_message(renderer, message);
    SDL_Delay(2000); // Hiển thị trong 2 giây trước khi kết thúc
}

void handle_login(SDL_Renderer *renderer, const char *username, const char *password) {
    if (login_user(username, password)) {
        show_message(renderer, "Login success!");
        SDL_Delay(2000); // Hiển thị thông báo trong 2 giây
        handle_input_number(renderer); // Chuyển sang phần nhập số
    } else {
        show_message(renderer, "Sai thông tin đăng nhập!");
    }
}

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    SDL_StartTextInput(); // Bắt đầu nhận văn bản
    connect_db();

    SDL_Window *window = SDL_CreateWindow("Đăng nhập/Đăng ký", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    char username[255], password[255];
    printf("Nhập tên người dùng: ");
    scanf("%s", username);
    printf("Nhập mật khẩu: ");
    scanf("%s", password);

    int option;
    printf("Chọn 1 để Đăng ký, 2 để Đăng nhập: ");
    scanf("%d", &option);

    SDL_Event e;
    int quit = 0;
    while (!quit) {
        // Xử lý sự kiện SDL
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }

        if (option == 1) {
            handle_register(renderer, username, password);
        } else if (option == 2) {
            handle_login(renderer, username, password);
        }

        SDL_Delay(100);  // Đợi một chút để giảm tải CPU
    }

    SDL_StopTextInput(); // Kết thúc nhận văn bản
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
    mysql_close(conn);
    return 0;
}
