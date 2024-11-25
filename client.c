#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <SDL2/SDL.h>
#include "protocol.h"
#include "ui/ui.h"

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct {
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char serverMessage[BUFFER_SIZE];
    int option; 
    int loggedIn;
     int sock;
} SharedData;

SharedData sharedData;


// Hàm gửi dữ liệu đến server và nhận phản hồi
void communicateWithServer(const char *message) {

     send(sharedData.sock, message, strlen(message), 0);

    memset(sharedData.serverMessage, 0, BUFFER_SIZE);
    int bytesRead = read(sharedData.sock, sharedData.serverMessage, BUFFER_SIZE);
    if(bytesRead <=0)
    {
         strcpy(sharedData.serverMessage, "Lost connection to server");
         SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", sharedData.serverMessage, NULL);

        close(sharedData.sock);
        exit(1);

    }



}

int main() {
    // Khởi tạo SDL và UI
    SDL_Window *window;
    SDL_Renderer *renderer;
    if (!ui_init(&window, &renderer)) {
        return 1;
    }

    struct sockaddr_in serv_addr;

    // Tạo socket
    if ((sharedData.sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation error");
         SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Socket creation error", NULL);
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Kết nối tới địa chỉ server
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
          SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Invalid address/ Address not supported", NULL);
        return -1;
    }

    // Kết nối tới server
    if (connect(sharedData.sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
         SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Connection Failed", NULL);
        return -1;
    }


    sharedData.loggedIn = 0;
    sharedData.option = 0; 
    SDL_Event e;
    int quit = 0;
    memset(sharedData.username, 0, BUFFER_SIZE);
    memset(sharedData.password, 0, BUFFER_SIZE);
    memset(sharedData.serverMessage, 0, BUFFER_SIZE);

     while (!quit) {
        while (SDL_PollEvent(&e) != 0) {

            ui_handle_input(&e, sharedData.username, sharedData.password, &sharedData.option, &quit);

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_RETURN) {
                if (sharedData.option == 1) {
                     communicateWithServer(REGISTER);
                    if (strcmp(sharedData.serverMessage, REGISTER_SUCCESS) == 0) {
                           sharedData.option = 2;  
                           memset(sharedData.username, 0, BUFFER_SIZE);
                           memset(sharedData.password, 0, BUFFER_SIZE);
                    }
                } else if (sharedData.option == 2) {
                    communicateWithServer(LOGIN);
                    if (strcmp(sharedData.serverMessage, LOGIN_SUCCESS) == 0) {
                        sharedData.loggedIn = 1;
                        sharedData.option = 3;
                        
                    }
                }
            }


        }
        // Vẽ giao diện dựa trên trạng thái hiện tại
            if(sharedData.loggedIn == 1)
            {   
                // ui_render_game_screen(renderer);


            } else
            { 
               ui_render_interface(renderer, sharedData.username, sharedData.password, sharedData.serverMessage, sharedData.option);
            }
     

        SDL_Delay(16); // Giới hạn FPS

    }



    close(sharedData.sock);
    ui_cleanup(window, renderer);
    return 0;
}