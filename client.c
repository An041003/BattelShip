#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "protocol.h"

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Tạo socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation error\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Kết nối tới địa chỉ server
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported\n");
        return -1;
    }

    // Kết nối tới server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed\n");
        return -1;
    }

    // Nhận yêu cầu từ server
    memset(buffer, 0, BUFFER_SIZE);  
    read(sock, buffer, BUFFER_SIZE);
    printf("%s", buffer);

    // Nhập lựa chọn (login hoặc register)
    memset(buffer, 0, BUFFER_SIZE);  
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = 0; 
    send(sock, buffer, strlen(buffer), 0);

    if (strcmp(buffer, REGISTER) == 0) {
        // Xử lý đăng ký
        memset(buffer, 0, BUFFER_SIZE);  
        read(sock, buffer, BUFFER_SIZE);  
        printf("%s", buffer);
        
        memset(buffer, 0, BUFFER_SIZE);  
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);  
        read(sock, buffer, BUFFER_SIZE);  
        printf("%s", buffer);

        memset(buffer, 0, BUFFER_SIZE);  
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        send(sock, buffer, strlen(buffer), 0);

        // Nhận phản hồi từ server
        memset(buffer, 0, BUFFER_SIZE);  
        read(sock, buffer, BUFFER_SIZE);
        printf("%s", buffer);

    } else if (strcmp(buffer, LOGIN) == 0) {
        // Xử lý đăng nhập
        memset(buffer, 0, BUFFER_SIZE);  
        read(sock, buffer, BUFFER_SIZE);  
        printf("%s", buffer);
        
        memset(buffer, 0, BUFFER_SIZE);
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        send(sock, buffer, strlen(buffer), 0);

        memset(buffer, 0, BUFFER_SIZE);  
        read(sock, buffer, BUFFER_SIZE);  
        printf("%s", buffer);
        
        memset(buffer, 0, BUFFER_SIZE);  
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = 0;
        send(sock, buffer, strlen(buffer), 0);

        // Nhận phản hồi từ server
        memset(buffer, 0, BUFFER_SIZE);  
        read(sock, buffer, BUFFER_SIZE);
        printf("%s", buffer);

        // Kiểm tra nếu đăng nhập thành công
        if (strstr(buffer, START_GAME) != NULL) {
            printf("\nYou are now logged in and connected to the game server!\n");
            // Logic game có thể được thêm vào đây
        } else {
            printf("Failed to log in. Exiting.\n");
        }
    }

    close(sock);
    return 0;
}