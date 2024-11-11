// auth.h
#ifndef AUTH_H
#define AUTH_H

#include <stdbool.h>

// Đường dẫn file lưu trữ thông tin người dùng
#define USER_DATA_FILE "user_data.txt"

// Hàm đăng ký tài khoản mới
bool register_account(const char *username, const char *password);

// Hàm đăng nhập
bool login_account(const char *username, const char *password, char *session_id);

// Hàm băm mật khẩu sử dụng SHA-256
void hash_password(const char *password, char *hashed_password);

#endif // AUTH_H
