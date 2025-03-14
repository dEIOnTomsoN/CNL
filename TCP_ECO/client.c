#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#include <unistd.h>  
#include <arpa/inet.h>  
#include <sys/socket.h>  

#define PORT 8080  
#define SIZE 1024  

int main() {  
    int sock;  
    struct sockaddr_in address;  
    char buffer[SIZE];  

    sock = socket(AF_INET, SOCK_STREAM, 0);  
    if (sock < 0) {
        perror("Socket creation error");
        exit(1);
    }

    memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;  
    address.sin_port = htons(PORT);  
    address.sin_addr.s_addr = INADDR_ANY;  

    if (connect(sock, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Connection failed");
        close(sock);
        exit(1);
    }

    printf("Connected to the server. Type messages to send:\n");

    while (1) {
        printf("Client: ");
        fgets(buffer, SIZE, stdin);
        send(sock, buffer, strlen(buffer), 0);

        ssize_t bytes_received = read(sock, buffer, SIZE);
        if (bytes_received <= 0) break;

        buffer[bytes_received] = '\0';
        printf("Echo from server: %s", buffer);
    }

    close(sock);
    return 0;
}

