cmake_minimum_required(VERSION 3.10)

project(MyProject LANGUAGES C)

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
    view/play_view.c
    view/result_view.c
    view/placeship_view.c 
    controller/register.c
    controller/login.c
    controller/ship_placement.c 
)

add_executable(Server
    server.c
    ${COMMON_SOURCES}
)
target_link_libraries(Server SDL2 SDL2_ttf SDL2_image ssl crypto mysqlclient)

add_executable(Client
    client.c
    ${COMMON_SOURCES}
)
target_link_libraries(Client SDL2 SDL2_ttf SDL2_image ssl crypto mysqlclient)
target_link_libraries(Server mysqlclient pthread)