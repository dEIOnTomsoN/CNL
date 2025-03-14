#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in server_addr;
    int num1, num2, sum;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter first number: ");
    scanf("%d", &num1);
    printf("Enter second number: ");
    scanf("%d", &num2);

    int converted_num1 = htonl(num1);
    int converted_num2 = htonl(num2);

    sendto(sockfd, &converted_num1, sizeof(converted_num1), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));
    sendto(sockfd, &converted_num2, sizeof(converted_num2), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

    int received_sum;
    recvfrom(sockfd, &received_sum, sizeof(received_sum), 0, NULL, NULL);
    sum = ntohl(received_sum);

    printf("The Sum is: %d\n", sum);

    close(sockfd);
    return 0;
}

