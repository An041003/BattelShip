#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
MYSQL *conn;

void connect_db() {
    conn = mysql_init(NULL);
    if (!conn) {
        printf("MySQL initialization failed\n");
        exit(1);
    }
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
    if (!stmt) {
        printf("Failed to initialize statement: %s\n", mysql_error(conn));
        return 0;
    }

    const char *query = "INSERT INTO users (username, password) VALUES (?, ?)";
    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        printf("Failed to prepare statement: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)username;
    bind[0].buffer_length = strlen(username);

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char *)password;
    bind[1].buffer_length = strlen(password);

    if (mysql_stmt_bind_param(stmt, bind)) {
        printf("Failed to bind parameters: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

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
    if (!stmt) {
        printf("Failed to initialize statement: %s\n", mysql_error(conn));
        return 0;
    }

    const char *query = "SELECT COUNT(*) FROM users WHERE username=? AND password=?";
    if (mysql_stmt_prepare(stmt, query, strlen(query))) {
        printf("Failed to prepare statement: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

    MYSQL_BIND bind[2];
    memset(bind, 0, sizeof(bind));

    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)username;
    bind[0].buffer_length = strlen(username);

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char *)password;
    bind[1].buffer_length = strlen(password);

    if (mysql_stmt_bind_param(stmt, bind)) {
        printf("Failed to bind parameters: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

    if (mysql_stmt_execute(stmt)) {
        printf("Failed to execute statement: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

    int login_success = 0;
    MYSQL_BIND result_bind;
    memset(&result_bind, 0, sizeof(result_bind));

    result_bind.buffer_type = MYSQL_TYPE_LONG;
    result_bind.buffer = &login_success;

    if (mysql_stmt_bind_result(stmt, &result_bind)) {
        printf("Failed to bind result: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

    if (mysql_stmt_fetch(stmt) == 1) {
        printf("Failed to fetch result: %s\n", mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return 0;
    }

    mysql_stmt_close(stmt);
    return login_success > 0;
}

void render_cursor(SDL_Renderer *renderer, int x, int y) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Màu trắng
    SDL_Rect cursor = {x, y, 2, 20}; // Kích thước dấu nháy
    SDL_RenderFillRect(renderer, &cursor);
}


void handle_mouse_motion(SDL_Event *e, SDL_Rect *input_boxes, int num_boxes, SDL_Rect *login_button, int *focus) {
    // Kiểm tra hover ô nhập liệu
    for (int i = 0; i < num_boxes; i++) {
        if (e->motion.x >= input_boxes[i].x && e->motion.x <= input_boxes[i].x + input_boxes[i].w &&
            e->motion.y >= input_boxes[i].y && e->motion.y <= input_boxes[i].y + input_boxes[i].h) {
            *focus = i;  // Focus vào ô
            return;
        }
    }

    // Kiểm tra hover nút login
    if (e->motion.x >= login_button->x && e->motion.x <= login_button->x + login_button->w &&
        e->motion.y >= login_button->y && e->motion.y <= login_button->y + login_button->h) {
        *focus = 2;  // Hover vào nút
        return;
    }

    *focus = -1;  // Không hover gì cả
}

int handle_mouse_click(SDL_Event *e, SDL_Rect *input_boxes, int num_boxes) {
    for (int i = 0; i < num_boxes; i++) {
        if (e->button.x >= input_boxes[i].x && e->button.x <= input_boxes[i].x + input_boxes[i].w &&
            e->button.y >= input_boxes[i].y && e->button.y <= input_boxes[i].y + input_boxes[i].h) {
            return i;  // Trả về chỉ số của ô được chọn
        }
    }
    return -1;  // Không có ô nào được chọn
}

void draw_rounded_box(SDL_Renderer *renderer, SDL_Rect rect, SDL_Color color, int radius) {
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // Vẽ thân chính
    SDL_Rect main_rect = {rect.x + radius, rect.y, rect.w - 2 * radius, rect.h};
    SDL_RenderFillRect(renderer, &main_rect);

    SDL_Rect left_rect = {rect.x, rect.y + radius, radius, rect.h - 2 * radius};
    SDL_Rect right_rect = {rect.x + rect.w - radius, rect.y + radius, radius, rect.h - 2 * radius};
    SDL_RenderFillRect(renderer, &left_rect);
    SDL_RenderFillRect(renderer, &right_rect);

    // Vẽ góc tròn
    for (int w = 0; w < radius; w++) {
        SDL_RenderDrawLine(renderer, rect.x + radius - w, rect.y + w, rect.x + rect.w - radius + w, rect.y + w);
        SDL_RenderDrawLine(renderer, rect.x + radius - w, rect.y + rect.h - w - 1, rect.x + rect.w - radius + w, rect.y + rect.h - w - 1);
    }
}

void handle_mouse_event(SDL_Event *e, int *option, char *message, const char *username, const char *password) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
        int x = e->button.x;
        int y = e->button.y;

        // Kiểm tra xem có nhấn vào nút Register không
        if (x >= 100 && x <= 250 && y >= 250 && y <= 300) {
            *option = 1;  // Register
            if (register_user(username, password)) {
                strcpy(message, "Registration successful!");
            } else {
                strcpy(message, "Registration failed!");
            }
        }

        // Kiểm tra xem có nhấn vào nút Login không
        if (x >= 300 && x <= 450 && y >= 250 && y <= 300) {
            *option = 2;  // Login
            if (login_user(username, password)) {
                strcpy(message, "Login successful!");
            } else {
                strcpy(message, "Login failed!");
            }
        }
    }
}

void handle_input(SDL_Event *e, char *buffer, int max_length, int *focus) {
    if (e->type == SDL_TEXTINPUT && *focus >= 0) {
        if (strlen(buffer) < max_length - 1) {
            strcat(buffer, e->text.text);
        }
    } else if (e->type == SDL_KEYDOWN) {
        if (e->key.keysym.sym == SDLK_BACKSPACE && strlen(buffer) > 0) {
            buffer[strlen(buffer) - 1] = '\0';
        } else if (e->key.keysym.sym == SDLK_TAB) {
            *focus = (*focus + 1) % 3; // Chuyển focus (vòng tròn qua các trường)
        }
    }
}

void render_text(SDL_Renderer *renderer, const char *text, int x, int y) {
    TTF_Font *font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        printf("Unable to load font: %s\n", TTF_GetError());
        return;
    }

    if (strlen(text) == 0) {
        text = " ";  // Nếu chuỗi rỗng, thay thế bằng một khoảng trắng
    }

    SDL_Color color = {255, 255, 255};
    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
    if (!surface) {
        printf("Unable to create text surface: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        printf("Unable to create text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(surface);
        TTF_CloseFont(font);
        return;
    }

    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, NULL, &dest);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
    TTF_CloseFont(font);
}

void render_option_screen(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 50, 50, 80, 255);  // Màu xanh đậm hơn
    SDL_RenderClear(renderer);

    render_text(renderer, "Welcome! Choose an option:", 180, 100);

    SDL_Rect login_button = {220, 150, 200, 50};
    SDL_Color login_color = {60, 120, 200, 255};
    draw_rounded_box(renderer, login_button, login_color, 10);
    render_text(renderer, "1. Login", 280, 160);

    SDL_Rect register_button = {220, 220, 200, 50};
    SDL_Color register_color = {80, 150, 100, 255};
    draw_rounded_box(renderer, register_button, register_color, 10);
    render_text(renderer, "2. Register", 260, 230);

    SDL_RenderPresent(renderer);
}

void render_game_start_screen(SDL_Renderer *renderer) {
    SDL_SetRenderDrawColor(renderer, 30, 50, 70, 255);  // Màu nền tối
    SDL_RenderClear(renderer);

    render_text(renderer, "Game Start!", 240, 200);

    SDL_RenderPresent(renderer);
}

void render_interface(SDL_Renderer *renderer, const char *username, const char *password, int focus, int option) {
    // Đặt màu nền
    SDL_SetRenderDrawColor(renderer, 40, 40, 60, 255);
    SDL_RenderClear(renderer);

    SDL_Color input_box_color = {60, 80, 120, 255};
    SDL_Color hover_color = {80, 100, 150, 255};
    SDL_Color text_color = {255, 255, 255, 255};

    // Tiêu đề
    render_text(renderer, option == 1 ? "User Login" : "User Registration", 240, 30);

    // Vẽ hộp nhập liệu username
    SDL_Rect username_box = {200, 100, 240, 40};
    draw_rounded_box(renderer, username_box, focus == 0 ? hover_color : input_box_color, 10);
    render_text(renderer, "Username:", 200, 75);
    render_text(renderer, username, 210, 110);

    if (focus == 0) {
        int cursor_x = 210 + strlen(username) * 12; // Xác định vị trí dấu nháy
        render_cursor(renderer, cursor_x, 110);
    }

    // Vẽ hộp nhập liệu password
    SDL_Rect password_box = {200, 170, 240, 40};
    draw_rounded_box(renderer, password_box, focus == 1 ? hover_color : input_box_color, 10);
    render_text(renderer, "Password:", 200, 145);
    render_text(renderer, password, 210, 180);

    if (focus == 1) {
        int cursor_x = 210 + strlen(password) * 12; // Xác định vị trí dấu nháy
        render_cursor(renderer, cursor_x, 180);
    }

    // Nút xác nhận (Login/Register)
    SDL_Rect confirm_button = {220, 250, 200, 50};
    SDL_Color button_color = focus == 2 ? hover_color : input_box_color;
    draw_rounded_box(renderer, confirm_button, button_color, 10);
    render_text(renderer, option == 1 ? "Login" : "Register", 280, 260);

    SDL_RenderPresent(renderer);
}


// Các hàm hiển thị giao diện không thay đổi, tiếp tục sử dụng như cũ
int main(int argc, char *argv[]) {
    // Khởi tạo SDL và TTF
    if (SDL_Init(SDL_INIT_VIDEO) < 0 || TTF_Init() < 0) {
        printf("Initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Kết nối cơ sở dữ liệu
    connect_db();

    // Tạo cửa sổ và renderer
    SDL_Window *window = SDL_CreateWindow("Login/Register", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_StartTextInput();

    // Biến lưu trữ thông tin
    char username[255] = "";
    char password[255] = "";
    int option = -1;      // -1: Màn hình chọn, 1: Đăng nhập, 2: Đăng ký
    int logged_in = 0;    // Cờ đăng nhập thành công
    int focus = 0;        // 0: Username, 1: Password, 2: Button
    int quit = 0;

    // Vùng ô nhập liệu
    SDL_Rect input_boxes[2] = {{200, 100, 240, 40}, {200, 170, 240, 40}};
    SDL_Rect confirm_button = {220, 250, 200, 50};  // Nút xác nhận
    SDL_Event e;

    // Vòng lặp chính
    while (!quit) {
        // Hiển thị màn hình tương ứng với trạng thái
        if (option == -1) {
            render_option_screen(renderer);  // Hiển thị màn hình chọn (Login/Register)
        } else if (logged_in) {
            render_game_start_screen(renderer);  // Hiển thị màn hình "Game Start"
        } else {
            render_interface(renderer, username, password, focus, option);  // Hiển thị giao diện đăng nhập/đăng ký
        }

        // Xử lý sự kiện
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;  // Thoát chương trình
            } else if (option == -1) {
                // Xử lý màn hình chọn
                if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (e.button.x >= 220 && e.button.x <= 420 && e.button.y >= 150 && e.button.y <= 200) {
                        option = 1;  // Chọn "Login"
                        memset(username, 0, sizeof(username));  // Reset thông tin nhập
                        memset(password, 0, sizeof(password));
                        focus = 0;  // Đặt focus vào ô username
                    } else if (e.button.x >= 220 && e.button.x <= 420 && e.button.y >= 220 && e.button.y <= 270) {
                        option = 2;  // Chọn "Register"
                        memset(username, 0, sizeof(username));  // Reset thông tin nhập
                        memset(password, 0, sizeof(password));
                        focus = 0;  // Đặt focus vào ô username
                    }
                }
            } else if (option == 1 || option == 2) {
                // Xử lý màn hình đăng nhập/đăng ký
                if (e.type == SDL_MOUSEMOTION) {
                    handle_mouse_motion(&e, input_boxes, 2, &confirm_button, &focus);  // Xử lý hover
                } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                    if (focus == 0) {
                        focus = 0;  // Focus vào ô username
                    } else if (focus == 1) {
                        focus = 1;  // Focus vào ô password
                    } else if (focus == 2) {
                        if (option == 1) {
                            // Xử lý Login
                            if (login_user(username, password)) {
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Login Successful", "You have logged in successfully!", NULL);
                                logged_in = 1;
                            } else {
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Login Failed", "Invalid username or password. Please try again.", NULL);
                                memset(username, 0, sizeof(username));  // Reset thông tin nhập
                                memset(password, 0, sizeof(password));
                                focus = 0;
                            }
                        } else if (option == 2) {
                            // Xử lý Register
                            if (register_user(username, password)) {
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, "Registration Successful", "Account created successfully!", NULL);
                                option = 1;  // Chuyển về chế độ đăng nhập
                                memset(username, 0, sizeof(username));  // Reset thông tin nhập
                                memset(password, 0, sizeof(password));
                                focus = 0;
                            } else {
                                SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Registration Failed", "Failed to create account. Please try again.", NULL);
                            }
                        }
                    }
                } else if (e.type == SDL_TEXTINPUT || e.type == SDL_KEYDOWN) {
                    if (focus == 0) {
                        handle_input(&e, username, 255, &focus);  // Nhập username
                    } else if (focus == 1) {
                        handle_input(&e, password, 255, &focus);  // Nhập password
                    }
                }
            }
        }
    }

    // Dọn dẹp và kết thúc
    SDL_StopTextInput();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    mysql_close(conn);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
