cmake_minimum_required(VERSION 3.10)

project(MyProject LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(COMMON_SOURCES
    auth.c
    ui/ui.c 
)

add_executable(Server
    server.c
    ${COMMON_SOURCES}
)
target_link_libraries(Server SDL2 SDL2_ttf ssl crypto mysqlclient)

add_executable(Client
    client.c
    ${COMMON_SOURCES}
)
target_link_libraries(Client SDL2 SDL2_ttf ssl crypto mysqlclient)
