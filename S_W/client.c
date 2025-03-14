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
    struct sockaddr_in server_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];
    int num_packets, packet_count = 1;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Enter the number of packets to send: ");
    scanf("%d", &num_packets);

    while (packet_count <= num_packets) {
        sprintf(buffer, "%d", packet_count);
        printf("Sending packet: %s\n", buffer);
        sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, sizeof(server_addr));

        addr_size = sizeof(server_addr);
        bzero(buffer, BUFFER_SIZE);
        int rec = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &addr_size);

        if (rec < 0) {
            printf("Timeout! Resending packet %d\n", packet_count);
        } else {
            printf("Acknowledgment received for packet %s\n", buffer);
            packet_count++;
        }
        sleep(1);
    }

    close(sockfd);
    return 0;
}

