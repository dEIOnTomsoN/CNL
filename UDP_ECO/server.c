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
    struct sockaddr_in serv_addr, cli_addr;  
    socklen_t cli_len = sizeof(cli_addr);  

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);  
    if (sockfd < 0) {
        perror("Error opening socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;  
    serv_addr.sin_addr.s_addr = INADDR_ANY;  
    serv_addr.sin_port = htons(PORT);  

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    printf("UDP Echo Server listening on port %d...\n", PORT);

    while (1) {  
        memset(buffer, 0, BUFFER_SIZE);  
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&cli_addr, &cli_len);
        if (n < 0) {
            perror("Error receiving data");
            break;
        }

        printf("Received: %s", buffer);
        sendto(sockfd, buffer, n, 0, (struct sockaddr*)&cli_addr, cli_len);
    }

    close(sockfd);
    return 0;
}

