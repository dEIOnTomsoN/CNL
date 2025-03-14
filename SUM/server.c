#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t len;
    int num1, num2, sum;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        exit(1);
    }

    printf("Server is listening on port %d...\n", PORT);
    len = sizeof(client_addr);

    recvfrom(sockfd, &num1, sizeof(num1), 0, (struct sockaddr*)&client_addr, &len);
    num1 = ntohl(num1);

    recvfrom(sockfd, &num2, sizeof(num2), 0, (struct sockaddr*)&client_addr, &len);
    num2 = ntohl(num2);

    printf("Received numbers: %d and %d\n", num1, num2);
    sum = num1 + num2;
    int converted_sum = htonl(sum);

    sendto(sockfd, &converted_sum, sizeof(converted_sum), 0, (struct sockaddr*)&client_addr, len);
    printf("Sum sent back to client: %d\n", sum);

    close(sockfd);
    return 0;
}

