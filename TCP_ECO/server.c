#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
#include <sys/socket.h>  

#define PORT 8080  
#define SIZE 1024  

int main() {  
    int sockfd, newfd;  
    struct sockaddr_in address;  
    char buffer[SIZE];  
    socklen_t addr_len;  

    sockfd = socket(AF_INET, SOCK_STREAM, 0);  
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;  
    address.sin_port = htons(PORT);  
    address.sin_addr.s_addr = INADDR_ANY;  

    if (bind(sockfd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    if (listen(sockfd, 3) < 0) {
        perror("Listen failed");
        close(sockfd);
        exit(1);
    }

    printf("Echo server listening on port %d...\n", PORT);
    addr_len = sizeof(address);

    newfd = accept(sockfd, (struct sockaddr*)&address, &addr_len);
    if (newfd < 0) {
        perror("Accept failed");
        close(sockfd);
        exit(1);
    }

    printf("Connection established\n");

    while (1) {
        ssize_t bytes_read = read(newfd, buffer, SIZE);
        if (bytes_read <= 0) break;

        buffer[bytes_read] = '\0';
        printf("Received: %s", buffer);
        send(newfd, buffer, bytes_read, 0);
    }

    printf("Connection closed.\n");
    close(newfd);
    close(sockfd);
    return 0;
}

