#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 5568
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];
    int n;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    printf("Stop-and-Wait Server started. Listening on port %d...\n", PORT);
    addr_size = sizeof(client_addr);

    while (1) {
        bzero(buffer, BUFFER_SIZE);
        n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &addr_size);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }

        printf("Received packet: %s\n", buffer);
        sleep(1);  // Simulate delay

        sendto(sockfd, buffer, n, 0, (struct sockaddr*)&client_addr, addr_size);
        printf("Acknowledgment sent for packet: %s\n", buffer);
    }

    close(sockfd);
    return 0;
}

