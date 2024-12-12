cmake_minimum_required(VERSION 3.10)

project(Battleship LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)

set(COMMON_SOURCES
    model/auth.c
    model/network.c
    model/match.c
    model/board.c
    view/base_view.c
    view/login_view.c
    view/register_view.c
    view/menu_view.c
    view/home_view.c
    view/placeship_view.c
    view/temp.c
    view/fire_result_view.c
    controller/register.c
    controller/login.c
    controller/ship_placement.c
)

# Thêm mục tiêu Server
add_executable(Server
    server.c
    ${COMMON_SOURCES}
)
target_link_libraries(Server SDL2 SDL2_ttf SDL2_image ssl crypto mysqlclient pthread)

# Thêm mục tiêu Client
add_executable(Client
    client.c
    ${COMMON_SOURCES}
)
target_link_libraries(Client SDL2 SDL2_ttf SDL2_image ssl crypto mysqlclient pthread)

# Đảm bảo kiểm tra các thư viện cần thiết
find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_image REQUIRED)

target_include_directories(Client PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)

target_include_directories(Server PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
)
