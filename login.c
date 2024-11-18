#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>

// Kết nối MySQL
MYSQL *conn;

void connect_db() {
    conn = mysql_init(NULL);
    if (!mysql_real_connect(conn, "localhost", "root", "your_new_password", "user_db", 0, NULL, 0)) {
        printf("Database connection failed: %s\n", mysql_error(conn));
        exit(1);
    } else {
        printf("Database connected successfully!\n");
    }
}

// Hàm đăng ký
int register_user(const char *username, const char *password) {
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    const char *query = "INSERT INTO users (username, password) VALUES (?, ?)";
    if (!stmt) {
        printf("Failed to initialize statement: %s\n", mysql_error(conn));
        return 0;
    }
    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        printf("Failed to prepare statement: %s\n", mysql_stmt_error(stmt));
        return 0;
    }

    MYSQL_BIND bind[2] = {0};
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)username;
    bind[0].buffer_length = strlen(username);

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char *)password;
    bind[1].buffer_length = strlen(password);

    mysql_stmt_bind_param(stmt, bind);
    if (mysql_stmt_execute(stmt)) {
        printf("Statement execution failed: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

    mysql_stmt_close(stmt);
    return 1;
}

// Hàm đăng nhập
int login_user(const char *username, const char *password) {
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    const char *query = "SELECT COUNT(*) FROM users WHERE username=? AND password=?";
    if (!stmt) {
        printf("Failed to initialize statement: %s\n", mysql_error(conn));
        return 0;
    }
    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        printf("Failed to prepare statement: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

    MYSQL_BIND bind[2] = {0};
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)username;
    bind[0].buffer_length = strlen(username);

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char *)password;
    bind[1].buffer_length = strlen(password);

    mysql_stmt_bind_param(stmt, bind);

    if (mysql_stmt_execute(stmt)) {
        printf("Failed to execute statement: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

    int login_success = 0;
    MYSQL_BIND result_bind = {0};
    result_bind.buffer_type = MYSQL_TYPE_LONG;
    result_bind.buffer = &login_success;

    mysql_stmt_bind_result(stmt, &result_bind);
    mysql_stmt_fetch(stmt);
    mysql_stmt_close(stmt);

    return login_success > 0;
}

// Chức năng hiển thị giao diện
void render_text(SDL_Renderer *renderer, const char *text, int x, int y) {
    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);  // Cập nhật đường dẫn phù hợp
    if (!font) {
        printf("Unable to load font: %s\n", TTF_GetError());
        return;
    }
    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

// Hiển thị giao diện chính
void render_interface(SDL_Renderer *renderer, const char *username, const char *password, const char *message, int option) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    render_text(renderer, "Register/Login", 250, 20);
    render_text(renderer, "Username:", 100, 100);
    render_text(renderer, username, 300, 100);
    render_text(renderer, "Password:", 100, 150);
    render_text(renderer, password, 300, 150);

    render_text(renderer, "1: Register", 100, 250);
    render_text(renderer, "2: Login", 100, 300);

    if (message) {
        render_text(renderer, message, 100, 350);
    }

    SDL_RenderPresent(renderer);
}

// Xử lý đầu vào người dùng
void handle_input(SDL_Event *e, char *buffer, int max_length) {
    if (e->type == SDL_TEXTINPUT) {
        if (strlen(buffer) < max_length - 1) {
            strcat(buffer, e->text.text);
        }
    } else if (e->type == SDL_KEYDOWN) {
        if (e->key.keysym.sym == SDLK_BACKSPACE && strlen(buffer) > 0) {
            buffer[strlen(buffer) - 1] = '\0';
        }
    }
}

int main(int argc, char *argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() < 0) {
        printf("TTF initialization failed: %s\n", TTF_GetError());
        SDL_Quit();
        return 1;
    }

    connect_db();

    SDL_Window *window = SDL_CreateWindow("Register/Login", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_StartTextInput();

    char username[255] = "";
    char password[255] = "";
    char message[255] = "";
    int option = 0;

    // Xử lý một sự kiện duy nhất
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        printf("Handling event\n");
        if (e.type == SDL_QUIT) {
            printf("Quitting application\n");
            break;
        } else if (e.type == SDL_KEYDOWN) {
            if (e.key.keysym.sym == SDLK_RETURN) {
                if (option == 1) {
                    if (register_user(username, password)) {
                        strcpy(message, "Registration successful!");
                    } else {
                        strcpy(message, "Registration failed!");
                    }
                } else if (option == 2) {
                    if (login_user(username, password)) {
                        strcpy(message, "Login successful!");
                    } else {
                        strcpy(message, "Login failed!");
                    }
                }
            } else if (e.key.keysym.sym == SDLK_1) {
                option = 1;
            } else if (e.key.keysym.sym == SDLK_2) {
                option = 2;
            }
        } else if (e.type == SDL_TEXTINPUT) {
            if (option == 0) continue;  // Yêu cầu chọn đăng ký/đăng nhập trước
            handle_input(&e, option == 1 ? username : password, 255);
        }
    }

    // Hiển thị giao diện một lần
    render_interface(renderer, username, password, message, option);
    SDL_Delay(1000);

    // Dọn dẹp
    SDL_StopTextInput();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    mysql_close(conn);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
