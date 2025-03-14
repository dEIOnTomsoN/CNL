#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <arpa/inet.h>  

#define PORT 8080  
#define BUFFER_SIZE 1024  

int main() {  
    int sockfd;  
    char buffer[BUFFER_SIZE];  
    struct sockaddr_in serv_addr;  
    socklen_t serv_len = sizeof(serv_addr);  

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_port = htons(PORT);  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  

    printf("UDP Echo Client started. Type messages to send:\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        printf("Client: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&serv_addr, serv_len);

        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&serv_addr, &serv_len);
        if (n < 0) {
            perror("Error receiving data");
            break;
        }

        buffer[n] = '\0';
        printf("Echo from server: %s", buffer);
    }

    close(sockfd);
    return 0;
}

